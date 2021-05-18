#include "Textures/VideoTexture.hpp"

VideoTexture::VideoTexture(SDL_Renderer* renderer) {
  // Initialize
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
  _gRenderer = renderer;
}

VideoTexture::VideoTexture() {
  // Initialize
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
}

void VideoTexture::setRenderer(SDL_Renderer* renderer) { _gRenderer = renderer; }


int refresh_video(void *opaque){
	int thread_exit=0;
	while (thread_exit==0) {
		SDL_Event event;
		event.type = SDL_USEREVENT+1;
		SDL_PushEvent(&event);
		SDL_Delay(40);
	}
	thread_exit=0;
	//Break
	SDL_Event event;
	event.type = SDL_USEREVENT+2;
	SDL_PushEvent(&event);
	return 0;
}

int VideoTexture::startPlaying()
{
	// if(SDL_Init(SDL_INIT_VIDEO)) {  
	// 	printf( "Could not initialize SDL - %s\n", SDL_GetError()); 
	// 	return -1;
	// } 

	SDL_Window *screen; 
	//SDL 2.0 Support for multiple windows

	Uint32 pixformat=0;
	//IYUV: Y + U + V  (3 planes)
	//YV12: Y + V + U  (3 planes)
	pixformat= SDL_PIXELFORMAT_IYUV;  

	SDL_Texture* sdlTexture = SDL_CreateTexture(_gRenderer,pixformat, SDL_TEXTUREACCESS_STREAMING,pixel_w,pixel_h);

	FILE *fp=NULL;
	fp=fopen("test_yuv420p_320x180.yuv","rb+");

	if(fp==NULL){
		printf("cannot open this file\n");
		return -1;
	}

	SDL_Rect sdlRect;  

	SDL_Thread *refresh_thread = SDL_CreateThread(refresh_video,NULL,NULL);
    
	SDL_Event event;
	while(1){
		//Wait
		SDL_WaitEvent(&event);
		if(event.type==SDL_USEREVENT+1){
			if (fread(buffer, 1, pixel_w*pixel_h*bpp/8, fp) != pixel_w*pixel_h*bpp/8){
				// Loop
				fseek(fp, 0, SEEK_SET);
				fread(buffer, 1, pixel_w*pixel_h*bpp/8, fp);
			}

			SDL_UpdateTexture( sdlTexture, NULL, buffer, pixel_w);  

			//FIX: If window is resize
			sdlRect.x = 0;  
			sdlRect.y = 0;  
			sdlRect.w = screen_w;  
			sdlRect.h = screen_h;  
			
			// SDL_RenderClear( _gRenderer );   
			SDL_RenderCopy( _gRenderer, sdlTexture, NULL, &sdlRect);  
			// SDL_RenderPresent( _gRenderer );  
			
		}else if(event.type==SDL_QUIT){
			thread_exit=1;
		}else if(event.type==SDL_USEREVENT+2){
			break;
		}
	}
	SDL_Quit();
	return 0;
}



void VideoTexture::stop(){
    thread_exit = 1;
}