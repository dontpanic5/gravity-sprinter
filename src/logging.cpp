#include "logging.h"

void batLog(const char * format, ...)
{
	va_list args;
	va_start(args, format);
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, format, args);
	va_end(args);
}