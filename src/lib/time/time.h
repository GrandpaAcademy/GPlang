#ifndef GPLANG_TIME_H
#define GPLANG_TIME_H

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/time.h>

// Time structures
typedef struct {
    int year;
    int month;      // 1-12
    int day;        // 1-31
    int hour;       // 0-23
    int minute;     // 0-59
    int second;     // 0-59
    int millisecond; // 0-999
    int microsecond; // 0-999
    int nanosecond;  // 0-999
    int weekday;    // 0-6 (Sunday = 0)
    int yearday;    // 1-366
    bool is_dst;    // Daylight saving time
    int timezone_offset; // Offset from UTC in minutes
} GPDateTime;

typedef struct {
    int64_t seconds;
    int32_t nanoseconds;
} GPDuration;

typedef struct {
    char* name;
    int offset_minutes;
    bool observes_dst;
} GPTimezone;

// Time creation and manipulation
GPDateTime gp_time_now(void);
GPDateTime gp_time_utc_now(void);
GPDateTime gp_time_create(int year, int month, int day, int hour, int minute, int second);
GPDateTime gp_time_from_timestamp(int64_t timestamp);
GPDateTime gp_time_from_string(const char* time_str, const char* format);
int64_t gp_time_to_timestamp(const GPDateTime* dt);
char* gp_time_to_string(const GPDateTime* dt, const char* format);

// Time arithmetic
GPDateTime gp_time_add_seconds(const GPDateTime* dt, int64_t seconds);
GPDateTime gp_time_add_minutes(const GPDateTime* dt, int64_t minutes);
GPDateTime gp_time_add_hours(const GPDateTime* dt, int64_t hours);
GPDateTime gp_time_add_days(const GPDateTime* dt, int64_t days);
GPDateTime gp_time_add_weeks(const GPDateTime* dt, int64_t weeks);
GPDateTime gp_time_add_months(const GPDateTime* dt, int64_t months);
GPDateTime gp_time_add_years(const GPDateTime* dt, int64_t years);
GPDateTime gp_time_add_duration(const GPDateTime* dt, const GPDuration* duration);

// Time comparison
int gp_time_compare(const GPDateTime* dt1, const GPDateTime* dt2);
bool gp_time_equals(const GPDateTime* dt1, const GPDateTime* dt2);
bool gp_time_before(const GPDateTime* dt1, const GPDateTime* dt2);
bool gp_time_after(const GPDateTime* dt1, const GPDateTime* dt2);
bool gp_time_between(const GPDateTime* dt, const GPDateTime* start, const GPDateTime* end);

// Duration operations
GPDuration gp_duration_create(int64_t seconds, int32_t nanoseconds);
GPDuration gp_duration_between(const GPDateTime* start, const GPDateTime* end);
GPDuration gp_duration_add(const GPDuration* d1, const GPDuration* d2);
GPDuration gp_duration_subtract(const GPDuration* d1, const GPDuration* d2);
GPDuration gp_duration_multiply(const GPDuration* d, double factor);
GPDuration gp_duration_divide(const GPDuration* d, double divisor);
double gp_duration_to_seconds(const GPDuration* d);
double gp_duration_to_minutes(const GPDuration* d);
double gp_duration_to_hours(const GPDuration* d);
double gp_duration_to_days(const GPDuration* d);
char* gp_duration_to_string(const GPDuration* d);

// Timezone operations
GPTimezone* gp_timezone_create(const char* name, int offset_minutes, bool observes_dst);
void gp_timezone_destroy(GPTimezone* tz);
GPDateTime gp_time_to_timezone(const GPDateTime* dt, const GPTimezone* tz);
GPDateTime gp_time_to_utc(const GPDateTime* dt);
GPTimezone* gp_timezone_local(void);
GPTimezone* gp_timezone_utc(void);
GPTimezone* gp_timezone_by_name(const char* name);

// Common timezone constants
extern const GPTimezone* GP_TZ_UTC;
extern const GPTimezone* GP_TZ_EST;
extern const GPTimezone* GP_TZ_PST;
extern const GPTimezone* GP_TZ_GMT;
extern const GPTimezone* GP_TZ_CET;
extern const GPTimezone* GP_TZ_JST;

// Time formatting and parsing
char* gp_time_format_iso8601(const GPDateTime* dt);
char* gp_time_format_rfc3339(const GPDateTime* dt);
char* gp_time_format_http(const GPDateTime* dt);
char* gp_time_format_custom(const GPDateTime* dt, const char* format);
GPDateTime gp_time_parse_iso8601(const char* str);
GPDateTime gp_time_parse_rfc3339(const char* str);
GPDateTime gp_time_parse_custom(const char* str, const char* format);

// Calendar operations
bool gp_time_is_leap_year(int year);
int gp_time_days_in_month(int year, int month);
int gp_time_days_in_year(int year);
int gp_time_week_of_year(const GPDateTime* dt);
int gp_time_day_of_year(const GPDateTime* dt);
GPDateTime gp_time_start_of_day(const GPDateTime* dt);
GPDateTime gp_time_end_of_day(const GPDateTime* dt);
GPDateTime gp_time_start_of_week(const GPDateTime* dt);
GPDateTime gp_time_end_of_week(const GPDateTime* dt);
GPDateTime gp_time_start_of_month(const GPDateTime* dt);
GPDateTime gp_time_end_of_month(const GPDateTime* dt);
GPDateTime gp_time_start_of_year(const GPDateTime* dt);
GPDateTime gp_time_end_of_year(const GPDateTime* dt);

// Business time operations
bool gp_time_is_weekend(const GPDateTime* dt);
bool gp_time_is_weekday(const GPDateTime* dt);
bool gp_time_is_business_hour(const GPDateTime* dt, int start_hour, int end_hour);
GPDateTime gp_time_next_business_day(const GPDateTime* dt);
GPDateTime gp_time_previous_business_day(const GPDateTime* dt);
int gp_time_business_days_between(const GPDateTime* start, const GPDateTime* end);

// Holiday detection (basic)
typedef struct {
    char* name;
    int month;
    int day;
    bool is_fixed;  // Fixed date or calculated (like Easter)
} GPHoliday;

bool gp_time_is_holiday(const GPDateTime* dt, const GPHoliday* holidays, int count);
GPDateTime gp_time_easter(int year);
GPDateTime gp_time_thanksgiving_us(int year);

// Stopwatch and timer functionality
typedef struct {
    struct timespec start_time;
    struct timespec end_time;
    bool is_running;
} GPStopwatch;

GPStopwatch* gp_stopwatch_create(void);
void gp_stopwatch_destroy(GPStopwatch* sw);
void gp_stopwatch_start(GPStopwatch* sw);
void gp_stopwatch_stop(GPStopwatch* sw);
void gp_stopwatch_reset(GPStopwatch* sw);
void gp_stopwatch_restart(GPStopwatch* sw);
GPDuration gp_stopwatch_elapsed(const GPStopwatch* sw);
double gp_stopwatch_elapsed_seconds(const GPStopwatch* sw);
double gp_stopwatch_elapsed_milliseconds(const GPStopwatch* sw);
double gp_stopwatch_elapsed_microseconds(const GPStopwatch* sw);

// High-precision timing
uint64_t gp_time_nanoseconds(void);
uint64_t gp_time_microseconds(void);
uint64_t gp_time_milliseconds(void);
double gp_time_seconds_precise(void);

// Sleep and delay functions
void gp_time_sleep_seconds(double seconds);
void gp_time_sleep_milliseconds(int64_t milliseconds);
void gp_time_sleep_microseconds(int64_t microseconds);
void gp_time_sleep_nanoseconds(int64_t nanoseconds);
void gp_time_sleep_until(const GPDateTime* target_time);

// Recurring events and scheduling
typedef enum {
    GP_RECUR_NONE,
    GP_RECUR_DAILY,
    GP_RECUR_WEEKLY,
    GP_RECUR_MONTHLY,
    GP_RECUR_YEARLY,
    GP_RECUR_CUSTOM
} GPRecurrenceType;

typedef struct {
    GPRecurrenceType type;
    int interval;           // Every N days/weeks/months/years
    int* weekdays;          // For weekly recurrence (0-6, Sunday=0)
    int weekday_count;
    int day_of_month;       // For monthly recurrence
    int month_of_year;      // For yearly recurrence
    GPDateTime start_date;
    GPDateTime end_date;    // Optional end date
    int max_occurrences;    // Optional max number of occurrences
} GPRecurrenceRule;

typedef struct {
    GPDateTime* dates;
    int count;
} GPDateList;

GPDateList gp_time_generate_recurrence(const GPRecurrenceRule* rule, 
                                      const GPDateTime* from, 
                                      const GPDateTime* to);
void gp_time_date_list_destroy(GPDateList* list);
GPDateTime gp_time_next_occurrence(const GPRecurrenceRule* rule, const GPDateTime* after);

// Age and period calculations
typedef struct {
    int years;
    int months;
    int days;
    int hours;
    int minutes;
    int seconds;
} GPPeriod;

GPPeriod gp_time_age(const GPDateTime* birth_date, const GPDateTime* current_date);
GPPeriod gp_time_period_between(const GPDateTime* start, const GPDateTime* end);
char* gp_time_period_to_string(const GPPeriod* period);

// Time zone database operations
typedef struct {
    char** zone_names;
    int count;
} GPTimezoneList;

GPTimezoneList gp_time_list_timezones(void);
void gp_time_timezone_list_destroy(GPTimezoneList* list);
bool gp_time_is_valid_timezone(const char* name);

// Astronomical time calculations
GPDateTime gp_time_sunrise(const GPDateTime* date, double latitude, double longitude);
GPDateTime gp_time_sunset(const GPDateTime* date, double latitude, double longitude);
GPDuration gp_time_daylight_duration(const GPDateTime* date, double latitude, double longitude);
double gp_time_solar_noon(const GPDateTime* date, double longitude);

// Performance timing utilities
typedef struct {
    const char* name;
    uint64_t start_time;
    uint64_t total_time;
    int call_count;
} GPProfileTimer;

GPProfileTimer* gp_time_profile_create(const char* name);
void gp_time_profile_destroy(GPProfileTimer* timer);
void gp_time_profile_start(GPProfileTimer* timer);
void gp_time_profile_end(GPProfileTimer* timer);
double gp_time_profile_average(const GPProfileTimer* timer);
void gp_time_profile_reset(GPProfileTimer* timer);
void gp_time_profile_report(const GPProfileTimer* timer);

// Utility functions
const char* gp_time_weekday_name(int weekday);
const char* gp_time_month_name(int month);
const char* gp_time_weekday_short_name(int weekday);
const char* gp_time_month_short_name(int month);
bool gp_time_is_valid_date(int year, int month, int day);
bool gp_time_is_valid_time(int hour, int minute, int second);
GPDateTime gp_time_min(const GPDateTime* dt1, const GPDateTime* dt2);
GPDateTime gp_time_max(const GPDateTime* dt1, const GPDateTime* dt2);

#endif // GPLANG_TIME_H
