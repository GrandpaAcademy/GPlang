#define _POSIX_C_SOURCE 199309L
#include "time.h"
#include <unistd.h>

// Month names
static const char* month_names[] = {
    "", "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

static const char* month_short_names[] = {
    "", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

// Weekday names
static const char* weekday_names[] = {
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

static const char* weekday_short_names[] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

// Time creation and manipulation
GPDateTime gp_time_now(void) {
    time_t now = time(NULL);
    struct tm* local_tm = localtime(&now);
    
    GPDateTime dt = {0};
    dt.year = local_tm->tm_year + 1900;
    dt.month = local_tm->tm_mon + 1;
    dt.day = local_tm->tm_mday;
    dt.hour = local_tm->tm_hour;
    dt.minute = local_tm->tm_min;
    dt.second = local_tm->tm_sec;
    dt.weekday = local_tm->tm_wday;
    dt.yearday = local_tm->tm_yday + 1;
    dt.is_dst = local_tm->tm_isdst > 0;
    
    // Get milliseconds
    struct timeval tv;
    gettimeofday(&tv, NULL);
    dt.millisecond = tv.tv_usec / 1000;
    dt.microsecond = tv.tv_usec % 1000;
    
    return dt;
}

GPDateTime gp_time_utc_now(void) {
    time_t now = time(NULL);
    struct tm* utc_tm = gmtime(&now);
    
    GPDateTime dt = {0};
    dt.year = utc_tm->tm_year + 1900;
    dt.month = utc_tm->tm_mon + 1;
    dt.day = utc_tm->tm_mday;
    dt.hour = utc_tm->tm_hour;
    dt.minute = utc_tm->tm_min;
    dt.second = utc_tm->tm_sec;
    dt.weekday = utc_tm->tm_wday;
    dt.yearday = utc_tm->tm_yday + 1;
    dt.is_dst = false;
    dt.timezone_offset = 0;
    
    struct timeval tv;
    gettimeofday(&tv, NULL);
    dt.millisecond = tv.tv_usec / 1000;
    dt.microsecond = tv.tv_usec % 1000;
    
    return dt;
}

GPDateTime gp_time_create(int year, int month, int day, int hour, int minute, int second) {
    GPDateTime dt = {0};
    dt.year = year;
    dt.month = month;
    dt.day = day;
    dt.hour = hour;
    dt.minute = minute;
    dt.second = second;
    
    // Calculate weekday
    struct tm tm = {0};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;
    
    mktime(&tm);
    dt.weekday = tm.tm_wday;
    dt.yearday = tm.tm_yday + 1;
    
    return dt;
}

GPDateTime gp_time_from_timestamp(int64_t timestamp) {
    time_t t = (time_t)timestamp;
    struct tm* tm = localtime(&t);
    
    GPDateTime dt = {0};
    dt.year = tm->tm_year + 1900;
    dt.month = tm->tm_mon + 1;
    dt.day = tm->tm_mday;
    dt.hour = tm->tm_hour;
    dt.minute = tm->tm_min;
    dt.second = tm->tm_sec;
    dt.weekday = tm->tm_wday;
    dt.yearday = tm->tm_yday + 1;
    dt.is_dst = tm->tm_isdst > 0;
    
    return dt;
}

int64_t gp_time_to_timestamp(const GPDateTime* dt) {
    if (!dt) return 0;
    
    struct tm tm = {0};
    tm.tm_year = dt->year - 1900;
    tm.tm_mon = dt->month - 1;
    tm.tm_mday = dt->day;
    tm.tm_hour = dt->hour;
    tm.tm_min = dt->minute;
    tm.tm_sec = dt->second;
    tm.tm_isdst = dt->is_dst ? 1 : 0;
    
    return (int64_t)mktime(&tm);
}

// Time arithmetic
GPDateTime gp_time_add_seconds(const GPDateTime* dt, int64_t seconds) {
    if (!dt) return (GPDateTime){0};
    
    int64_t timestamp = gp_time_to_timestamp(dt);
    return gp_time_from_timestamp(timestamp + seconds);
}

GPDateTime gp_time_add_minutes(const GPDateTime* dt, int64_t minutes) {
    return gp_time_add_seconds(dt, minutes * 60);
}

GPDateTime gp_time_add_hours(const GPDateTime* dt, int64_t hours) {
    return gp_time_add_seconds(dt, hours * 3600);
}

GPDateTime gp_time_add_days(const GPDateTime* dt, int64_t days) {
    return gp_time_add_seconds(dt, days * 86400);
}

// Time comparison
int gp_time_compare(const GPDateTime* dt1, const GPDateTime* dt2) {
    if (!dt1 || !dt2) return 0;
    
    int64_t ts1 = gp_time_to_timestamp(dt1);
    int64_t ts2 = gp_time_to_timestamp(dt2);
    
    if (ts1 < ts2) return -1;
    if (ts1 > ts2) return 1;
    
    // Compare sub-second precision
    if (dt1->millisecond < dt2->millisecond) return -1;
    if (dt1->millisecond > dt2->millisecond) return 1;
    
    if (dt1->microsecond < dt2->microsecond) return -1;
    if (dt1->microsecond > dt2->microsecond) return 1;
    
    if (dt1->nanosecond < dt2->nanosecond) return -1;
    if (dt1->nanosecond > dt2->nanosecond) return 1;
    
    return 0;
}

bool gp_time_equals(const GPDateTime* dt1, const GPDateTime* dt2) {
    return gp_time_compare(dt1, dt2) == 0;
}

bool gp_time_before(const GPDateTime* dt1, const GPDateTime* dt2) {
    return gp_time_compare(dt1, dt2) < 0;
}

bool gp_time_after(const GPDateTime* dt1, const GPDateTime* dt2) {
    return gp_time_compare(dt1, dt2) > 0;
}

// Duration operations
GPDuration gp_duration_create(int64_t seconds, int32_t nanoseconds) {
    GPDuration d = {seconds, nanoseconds};
    
    // Normalize nanoseconds
    while (d.nanoseconds >= 1000000000) {
        d.seconds++;
        d.nanoseconds -= 1000000000;
    }
    while (d.nanoseconds < 0) {
        d.seconds--;
        d.nanoseconds += 1000000000;
    }
    
    return d;
}

GPDuration gp_duration_between(const GPDateTime* start, const GPDateTime* end) {
    if (!start || !end) return (GPDuration){0, 0};
    
    int64_t start_ts = gp_time_to_timestamp(start);
    int64_t end_ts = gp_time_to_timestamp(end);
    
    int64_t seconds = end_ts - start_ts;
    
    // Calculate nanosecond difference
    int64_t start_ns = start->millisecond * 1000000 + start->microsecond * 1000 + start->nanosecond;
    int64_t end_ns = end->millisecond * 1000000 + end->microsecond * 1000 + end->nanosecond;
    int32_t nanoseconds = (int32_t)(end_ns - start_ns);
    
    return gp_duration_create(seconds, nanoseconds);
}

double gp_duration_to_seconds(const GPDuration* d) {
    if (!d) return 0.0;
    return (double)d->seconds + (double)d->nanoseconds / 1000000000.0;
}

double gp_duration_to_minutes(const GPDuration* d) {
    return gp_duration_to_seconds(d) / 60.0;
}

double gp_duration_to_hours(const GPDuration* d) {
    return gp_duration_to_seconds(d) / 3600.0;
}

double gp_duration_to_days(const GPDuration* d) {
    return gp_duration_to_seconds(d) / 86400.0;
}

// Calendar operations
bool gp_time_is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int gp_time_days_in_month(int year, int month) {
    static const int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (month < 1 || month > 12) return 0;
    
    if (month == 2 && gp_time_is_leap_year(year)) {
        return 29;
    }
    
    return days[month];
}

int gp_time_days_in_year(int year) {
    return gp_time_is_leap_year(year) ? 366 : 365;
}

bool gp_time_is_weekend(const GPDateTime* dt) {
    if (!dt) return false;
    return dt->weekday == 0 || dt->weekday == 6; // Sunday or Saturday
}

bool gp_time_is_weekday(const GPDateTime* dt) {
    return !gp_time_is_weekend(dt);
}

// Stopwatch functionality
GPStopwatch* gp_stopwatch_create(void) {
    GPStopwatch* sw = malloc(sizeof(GPStopwatch));
    if (!sw) return NULL;
    
    sw->is_running = false;
    memset(&sw->start_time, 0, sizeof(struct timespec));
    memset(&sw->end_time, 0, sizeof(struct timespec));
    
    return sw;
}

void gp_stopwatch_destroy(GPStopwatch* sw) {
    free(sw);
}

void gp_stopwatch_start(GPStopwatch* sw) {
    if (!sw) return;
    
    clock_gettime(CLOCK_MONOTONIC, &sw->start_time);
    sw->is_running = true;
}

void gp_stopwatch_stop(GPStopwatch* sw) {
    if (!sw || !sw->is_running) return;
    
    clock_gettime(CLOCK_MONOTONIC, &sw->end_time);
    sw->is_running = false;
}

void gp_stopwatch_reset(GPStopwatch* sw) {
    if (!sw) return;
    
    sw->is_running = false;
    memset(&sw->start_time, 0, sizeof(struct timespec));
    memset(&sw->end_time, 0, sizeof(struct timespec));
}

void gp_stopwatch_restart(GPStopwatch* sw) {
    gp_stopwatch_reset(sw);
    gp_stopwatch_start(sw);
}

double gp_stopwatch_elapsed_seconds(const GPStopwatch* sw) {
    if (!sw) return 0.0;
    
    struct timespec end_time;
    if (sw->is_running) {
        clock_gettime(CLOCK_MONOTONIC, &end_time);
    } else {
        end_time = sw->end_time;
    }
    
    double seconds = (double)(end_time.tv_sec - sw->start_time.tv_sec);
    double nanoseconds = (double)(end_time.tv_nsec - sw->start_time.tv_nsec);
    
    return seconds + nanoseconds / 1000000000.0;
}

double gp_stopwatch_elapsed_milliseconds(const GPStopwatch* sw) {
    return gp_stopwatch_elapsed_seconds(sw) * 1000.0;
}

// High-precision timing
uint64_t gp_time_nanoseconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}

uint64_t gp_time_microseconds(void) {
    return gp_time_nanoseconds() / 1000;
}

uint64_t gp_time_milliseconds(void) {
    return gp_time_nanoseconds() / 1000000;
}

double gp_time_seconds_precise(void) {
    return (double)gp_time_nanoseconds() / 1000000000.0;
}

// Sleep functions
void gp_time_sleep_seconds(double seconds) {
    if (seconds <= 0) return;
    
    struct timespec ts;
    ts.tv_sec = (time_t)seconds;
    ts.tv_nsec = (long)((seconds - ts.tv_sec) * 1000000000);
    
    nanosleep(&ts, NULL);
}

void gp_time_sleep_milliseconds(int64_t milliseconds) {
    gp_time_sleep_seconds((double)milliseconds / 1000.0);
}

void gp_time_sleep_microseconds(int64_t microseconds) {
    gp_time_sleep_seconds((double)microseconds / 1000000.0);
}

void gp_time_sleep_nanoseconds(int64_t nanoseconds) {
    gp_time_sleep_seconds((double)nanoseconds / 1000000000.0);
}

// Utility functions
const char* gp_time_weekday_name(int weekday) {
    if (weekday < 0 || weekday > 6) return "Unknown";
    return weekday_names[weekday];
}

const char* gp_time_month_name(int month) {
    if (month < 1 || month > 12) return "Unknown";
    return month_names[month];
}

const char* gp_time_weekday_short_name(int weekday) {
    if (weekday < 0 || weekday > 6) return "Unk";
    return weekday_short_names[weekday];
}

const char* gp_time_month_short_name(int month) {
    if (month < 1 || month > 12) return "Unk";
    return month_short_names[month];
}

bool gp_time_is_valid_date(int year, int month, int day) {
    if (year < 1 || month < 1 || month > 12 || day < 1) return false;
    return day <= gp_time_days_in_month(year, month);
}

bool gp_time_is_valid_time(int hour, int minute, int second) {
    return hour >= 0 && hour < 24 && minute >= 0 && minute < 60 && second >= 0 && second < 60;
}

// Time formatting
char* gp_time_format_iso8601(const GPDateTime* dt) {
    if (!dt) return NULL;
    
    char* result = malloc(32);
    if (!result) return NULL;
    
    snprintf(result, 32, "%04d-%02d-%02dT%02d:%02d:%02d",
             dt->year, dt->month, dt->day, dt->hour, dt->minute, dt->second);
    
    return result;
}
