#include <SDL3/SDL_filesystem.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SupergoonEngine/log.h>

#define MAX_LOG_SIZE 200

/**
 * @brief The file that will be written to when logs are put.
 *
 */
static FILE *openDebugFile = NULL;
/**
 * @brief The internal logging function that the others will end up calling.  Probably don't call it manually
 *
 * @param level The log level to log this as.
 * @param data_to_write The data to write to the log.
 */
static void Log(sgLogLevel level, const char *data_to_write);
/**
 * @brief The log level to log at, this should be sent in via settings.
 */
static sgLogLevel logLevel = Log_LInfo;
static char *_systemFilePath = NULL;
static const char *logFileName = "errors.log";
static int geGetFileFilepath(char *buffer, size_t bufferSize, const char *filename) {
	if (_systemFilePath != NULL) {
		snprintf(buffer, bufferSize, "%s%s", _systemFilePath, filename);
		return 0;
	}
	const char *base_path = SDL_GetPrefPath("Supergoon Games", "BbAdventures");
	int result = -1;  // Return -1 on error

	if (base_path == NULL) {
		// Use the current directory if SDL_GetBasePath() fails
		if (snprintf(buffer, bufferSize, "./%s", filename) < bufferSize) {
			result = 0;	 // Success
		}
	} else {
		// Construct the path using the base path provided by SDL
		if (snprintf(buffer, bufferSize, "%s%s", base_path, filename) < bufferSize) {
			_systemFilePath = strdup(base_path);
			SDL_free((void *)base_path);  // Clean up the SDL memory
			result = 0;					  // Success
		}
	}
	sgLogWarn("Path is %s", buffer);
	return result;
}

int sgInitializeDebugLogFile(void) {
	sgLogDebug("Opening log file at %s", logFileName);
	char buf[1000];
	// geGetLoadFilename(buf, sizeof(buf), logFileName);
	geGetFileFilepath(buf, sizeof(buf), logFileName);
	openDebugFile = fopen(buf, "a");
	if (openDebugFile)
		return 1;
	sgLogError("Could not open file for logging!");
	return 0;
}

int sgCloseDebugLogFile(void) {
	if (!openDebugFile)
		return 1;
	int result = fclose(openDebugFile);
	if (result)
		sgLogError("Couldn't close logging file.");
	return !result;
}
static void Log(sgLogLevel level, const char *thing_to_write) {
	time_t current_time;
	time(&current_time);
	struct tm *gm_time = gmtime(&current_time);
	char buf[30];
	strftime(buf, sizeof(buf), "%m-%d-%H:%M-%S", gm_time);
	FILE *outStream = level == Log_LError ? stderr : stdout;
	fprintf(outStream, "%s: %s end\n", buf, thing_to_write);
	if (level == Log_LError && openDebugFile) {
		fprintf(openDebugFile, "%s: %s\n", buf, thing_to_write);
	}
}
static void LogSetup(sgLogLevel level, const char *fmt, va_list args) {
	char buf[MAX_LOG_SIZE];
	vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);
	Log(level, buf);
}

static int ShouldLog(sgLogLevel level) {
#ifdef GN_RELEASE_BUILD
	return false;
#endif
	return logLevel <= level;
}

void sgLogDebug(const char *fmt, ...) {
	if (!ShouldLog(Log_LDebug))
		return;
	va_list args;
	va_start(args, fmt);
	LogSetup(Log_LDebug, fmt, args);
}

void sgLogInfo(const char *fmt, ...) {
	if (!ShouldLog(Log_LInfo))
		return;
	va_list args;
	va_start(args, fmt);
	LogSetup(Log_LInfo, fmt, args);
}

void sgLogWarn(const char *fmt, ...) {
	if (!ShouldLog(Log_LWarn))
		return;
	va_list args;
	va_start(args, fmt);
	LogSetup(Log_LWarn, fmt, args);
}

void sgLogError(const char *fmt, ...) {
	if (!ShouldLog(Log_LError))
		return;
	va_list args;
	va_start(args, fmt);
	LogSetup(Log_LError, fmt, args);
}

void sgLogCritical(const char *fmt, ...) {
	if (!ShouldLog(Log_LCritical))
		return;
	va_list args;
	va_start(args, fmt);
	LogSetup(Log_LCritical, fmt, args);
	fprintf(stderr, "Critical error, exiting!\n");
	exit(1);
}

void sgSetLogLevel(int newLevel) {
	logLevel = (sgLogLevel)newLevel;
}

