#include "SetupNetwork.hpp"
#include "InetAddr.hpp"
#include "SocketAcceptor.hpp"
#include "IOVec.hpp"
#include "Reactor.hpp"
#include "PollPolicy.hpp"
#include "EpollPolicy.hpp"
#include "NetHandler.hpp"
#include "Acceptor.hpp"
#include "Connector.hpp"
#include "Service.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>
#include <string.h>


/* #define SAMPLE_RATE  (17932) // Test failure to open with this value. */
#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (256)
#define NUM_SECONDS     (5)
#define NUM_CHANNELS    (2)
/* #define DITHER_FLAG     (paDitherOff) */
#define DITHER_FLAG     (0) /**/
/** Set to 1 if you want to capture the recording to a file. */
#define WRITE_TO_FILE   (0)

/* Select sample format. */
#if 1
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"
/*#elif 1
#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#elif 0
#define PA_SAMPLE_TYPE  paInt8
typedef char SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#else
#define PA_SAMPLE_TYPE  paUInt8
typedef unsigned char SAMPLE;
#define SAMPLE_SILENCE  (128)
#define PRINTF_S_FORMAT "%d"*/
#endif

#include	<list>

typedef EpollPolicy Policy;

typedef struct
{
    SAMPLE      *data;
    size_t	size;
}
paTestData;

class	Client;

std::list<paTestData*>		gllist;
bool				glclient = false;
Client				*gla = 0;

class	Client : public Service<>
{
public:
	Client()
	{
	}
	
  	virtual void	init()
	{
	  glclient = true;
	  std::cout << "client connected" << std::endl;
	  _reactor->registerHandler(_handle, *this, Reactor::WRITE);	
	}	

	virtual	int	handleInput(SOCKET handle)
	{
	  return 0;
	}

	virtual	int	handleOutput(SOCKET handle)	
	{
	  if (gllist.empty())
	  {
	    gla = this;
	    _reactor->registerHandler(handle, *this, Reactor::READ);
	    return 0;
	  }
	  paTestData	*data = gllist.front();
	  int ret  = this->send(reinterpret_cast<char*>(data->data), data->size);
	  if (ret != -1)
	  {
	    delete[] data->data;
	    delete data;
	    gllist.pop_front();
	  }
	  return ret;
	}

	virtual int	handleClose(SOCKET handle)
	{
		glclient = false;
		gllist.clear();
		this->close();
		_reactor->removeHandler(handle);
		delete this;
		return 0;
	}

private:
	 char	buffer[1024];
	 int	_size;
};

static int playCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData )
{
    Reactor	*reactor = (Reactor *)userData;

    (void) inputBuffer; /* Prevent unused variable warnings. */
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    size_t size =  framesPerBuffer * sizeof(SAMPLE);
    if (glclient)
    {
    	paTestData *data = new paTestData();
    	data->data = new SAMPLE[size];
    	data->size = size;
    	::memcpy(data->data, inputBuffer, size);
    	gllist.push_back(data);
	if (gllist.size() >= 1 && gla)
	  reactor->registerHandler(gla->getHandle(), *gla, Reactor::WRITE);
    }
    //::memset(outputBuffer, SAMPLE_SILENCE, size);
    return paContinue;
}


int  main(int ac, char **av)
{
  SetupNetwork  		init;
  InetAddr				test("4500");
  Reactor				*reactor = new Policy();
  Acceptor<Client>		acceptor;

  std::cout << "sdf" << std::endl;
  acceptor.setup(test, *reactor);
  /*Connector<Client>		client;
  client.setup(test, *reactor);*/
  PaError	err = Pa_Initialize();
  if (err != paNoError)
    return 1; 
  PaStreamParameters  inputParameters;
  PaStream*           stream;
  paTestData          data;

  inputParameters.device = Pa_GetDefaultInputDevice();
  if (inputParameters.device == paNoDevice)
	return -1;
  inputParameters.channelCount = 2;
  inputParameters.sampleFormat = PA_SAMPLE_TYPE;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;
  //err = Pa_OpenStream(&stream, &inputParameters, &inputParameters, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, playCallback, &data);
  err = Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, playCallback, reactor);
  err = Pa_StartStream(stream);
  reactor->waitForEvent();
  err = Pa_CloseStream(stream);
  Pa_Terminate();
  return (0);
}
