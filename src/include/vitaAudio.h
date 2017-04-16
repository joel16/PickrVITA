
#ifndef _VITAAUDIO_H
#define _VITAAUDIO_H


#include <stdint.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/kernel/sysmem.h>



#define RINGBUF_ERROR_NOT_INITIALIZED 	(-1)
#define RINGBUF_ERROR_INVALID_VALUE   	(-2)

#define RINGBUF_INIT_CREATE  (0x0001)

#define SCE_KERNEL_ATTR_SINGLE			(0x00000000U)
#define SCE_KERNEL_ATTR_MULTI			(0x00001000U)
#define SCE_KERNEL_ATTR_TH_FIFO			(0x00000000U)
#define SCE_KERNEL_ATTR_TH_PRIO			(0x00002000U)
#define SCE_KERNEL_ATTR_MS_FIFO			(0x00000000U)
#define SCE_KERNEL_ATTR_MS_PRIO			(0x00000000U)
#define SCE_KERNEL_ATTR_OPENABLE		(0x00000080U)

#define AUDIO_ERROR_OUT_OF_MEMORY 		(-1)

#define AUDIO_OUT_MAIN         			(0)
#define AUDIO_OUT_BGM          			(1)

#define AUDIO_STATUS_NONE      			(0)
#define AUDIO_STATUS_END       			(1)

#define AUDIO_READFLAG_READING 			(0)
#define AUDIO_READFLAG_READEND 		 	(1)

#define STEREO       					(2)
#define BUFNUM       					(2)
#define USER_GRAIN   					(256)

#define HEADER_SIZE  					(1024*32)
#define READBUF_SIZE 					(1024*32)
#define FILEBUF_SIZE 					(1024*1024)

#define SOUND_THREAD_NAME         		"soundThread"
#define READ_THREAD_NAME          		"readThread"

#define SOUND_THREAD_STACK_SIZE   		(4096 * 10)
#define READ_THREAD_STACK_SIZE    		(4096 * 10)

#define SCE_AUDIO_VOLUME_SHIFT	15
#define SCE_AUDIO_VOLUME_0dB			SCE_AUDIO_VOLUME_0DB	/* for PSP compatible */

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
#define SCE_KERNEL_DEFAULT_PRIORITY (static_cast<SceInt32>(0x10000100))
#else	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */
#define SCE_KERNEL_DEFAULT_PRIORITY ((SceInt32)0x10000100)
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#define SCE_KERNEL_DEFAULT_PRIORITY_USER SCE_KERNEL_DEFAULT_PRIORITY

#define SCE_KERNEL_MAX_CPU				(4)

#define SCE_KERNEL_CPU_MASK_SHIFT		(16)

#define SCE_KERNEL_CPU_MASK_USER_0		(0x01 << SCE_KERNEL_CPU_MASK_SHIFT)

#define SCE_KERNEL_CPU_MASK_USER_1		(0x02 << SCE_KERNEL_CPU_MASK_SHIFT)

#define SCE_KERNEL_CPU_MASK_USER_2		(0x04 << SCE_KERNEL_CPU_MASK_SHIFT)

#define SCE_KERNEL_CPU_MASK_USER_ALL	\
			(SCE_KERNEL_CPU_MASK_USER_0 | SCE_KERNEL_CPU_MASK_USER_1 | SCE_KERNEL_CPU_MASK_USER_2)


typedef struct FileBuffer
	{
	uint32_t				initFlag;
	SceUID					lock;
	SceKernelMutexOptParam 	optParam;
	uint8_t*				buf;
	uint32_t				write;
	uint32_t				read;
	uint32_t				count;
	uint32_t				size;
	} FileBuffer;

int32_t CreateBuffer	 		( FileBuffer* fileBuffer, uint8_t* buf, uint32_t size );
int32_t DeleteBuffer	 		( FileBuffer* fileBuffer );
int32_t ResetBuffer 	 		( FileBuffer* fileBuffer );
int32_t SetBufferData	 		( FileBuffer* fileBuffer, uint8_t* data, uint32_t setSize );
int32_t GetBufferData	 		( FileBuffer* fileBuffer, uint8_t* data, uint32_t getSize );
int32_t GetBufferCapacity		( FileBuffer* fileBuffer );
int32_t GetBufferCurrentCount	( FileBuffer* fileBuffer );


typedef struct {
	SceUID soundThread;
	SceUID readThread;
	SceUID fileHandle;
	uint8_t* header;
	uint8_t* buffer;
	uint8_t* readBuf;
	uint8_t* fillBuf;
	uint32_t dataSize;
	uint32_t offset;
	uint32_t readEnd;
	uint32_t endflag;
	uint32_t memory;
	uint32_t format;
	FileBuffer* fileBuffer;
	void* handle;
	uint32_t mode;
	uint32_t channels;
	uint32_t samplingRate;
	int16_t wavBuff[BUFNUM][USER_GRAIN * STEREO];
	} vitaAudio;















void vitaAudioPlayWavSound(vitaAudio* audio, const char* filename);
void vitaAudioPlayWavMusic(vitaAudio* audio, const char* filename);

void vitaAudioPlayOggSound(vitaAudio* audio, const char* filename);
void vitaAudioPlayOggMusic(vitaAudio* audio, const char* filename);

void vitaAudioStop(vitaAudio* audio);
	
int vitaAudioIsPlaying(vitaAudio* audio);

void vitaAudioFree(vitaAudio* audio);

#endif
