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
#define NUM_CHANNELS    (1)
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

std::list<paTestData*>		gllist;

class	Client : public Service<>
{
public:
	Client()
	{
	}
	
  	virtual void	init()
	{
	}	

	virtual	int	handleInput(SOCKET handle)
	{
	  paTestData    *data = new paTestData();
	  data->size = FRAMES_PER_BUFFER * NUM_CHANNELS * sizeof(SAMPLE);
	  data->data = new SAMPLE[data->size];
	  int	ret = this->recv(reinterpret_cast<char *>(data->data), data->size);
	  if (ret > 0)
	  {
	     gllist.push_back(data);     
	  }
	  return ret;
	}

	virtual	int	handleOutput(SOCKET handle)
	{
	  return 0;
	}

	virtual int	handleClose(SOCKET handle)
	{
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
    SAMPLE const *rptr = (SAMPLE const*)inputBuffer;
    SAMPLE *wptr = (SAMPLE*)outputBuffer;

    (void) inputBuffer; /* Prevent unused variable warnings. */
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    if (gllist.empty())
    { 
      size_t size =  framesPerBuffer * NUM_CHANNELS * sizeof(SAMPLE);
      ::memset(outputBuffer, SAMPLE_SILENCE, size);
    }
    else
    {
    	paTestData *data = gllist.front();
        ::memcpy(outputBuffer, data->data, data->size);
	delete[] data->data;
	delete data;
        gllist.pop_front();
    }
    return paContinue;
}


int  main(int ac, char **av)
{
  SetupNetwork  		init;
  InetAddr				test(av[1], av[2]);
  Reactor				*reactor = new Policy();
/*  Acceptor<Client>		acceptor;

  acceptor.setup(test, *reactor);*/
  Connector<Client>		client;
  client.setup(test, *reactor);
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
  err = Pa_OpenDefaultStream(&stream, 0, NUM_CHANNELS, paFloat32, 
SAMPLE_RATE, FRAMES_PER_BUFFER, playCallback, &data);
  err = Pa_StartStream(stream);
  reactor->waitForEvent();
  err = Pa_CloseStream(stream);
  Pa_Terminate();
  return (0);
}
