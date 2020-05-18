/**
 * Copyright (C) 2016 Jack Engqvist Johansson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef INC_YUAREL_H
#define INC_YUAREL_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The struct where the parsed values will be stored:
 *
 * scheme ":" [ "//" ] [ username ":" password "@" ] host [ ":" port ] [ "/" ] [ path ] [ "?" query ]
 *
 * Note: to make sure that no strings are copied, the first slash "/" in the
 * path will be used to null terminate the hostname if no port is supplied.
 */
struct yuarel {
	char *scheme; /* scheme, without ":" and "//" */
	char *username; /* username, default: NULL */
	char *password; /* password, default: NULL */
	char *host; /* hostname or IP address */
	int port; /* port, default: 0 */
	char *path; /* path, without leading "/", default: NULL */
	char *query; /* query, default: NULL */
	char *fragment; /* fragment, default: NULL */
};

/* A struct to hold the query string parameter values. */
struct yuarel_param {
	char *key;
	char *val;
};

/**
 * Parse a URL to a struct.
 *
 * The URL string should be in one of the following formats:
 *
 * Absolute URL:
 * scheme ":" [ "//" ] [ username ":" password "@" ] host [ ":" port ] [ "/" ] [ path ] [ "?" query ] [ "#" fragment ]
 *
 * Relative URL:
 * path [ "?" query ] [ "#" fragment ]
 *
 * The following parts will be parsed to the corresponding struct member.
 *
 * *url:     a pointer to the struct where to store the parsed values.
 * *url_str: a pointer to the url to be parsed (null terminated). The string
 *           will be modified.
 *
 * Returns 0 on success, otherwise -1.
 */
extern int yuarel_parse(struct yuarel *url, char *url_str);

/**
 * Split a path into several strings.
 *
 * No data is copied, the slashed are used as null terminators and then
 * pointers to each path part will be stored in **parts. Double slashes will be
 * treated as one.
 *
 * *path:     the path to split. The string will be modified.
 * **parts:   a pointer to an array of (char *) where to store the result.
 * max_parts: max number of parts to parse.
 *
 * Returns the number of parsed items. -1 on error.
 */
extern int yuarel_split_path(char *path, char **parts, int max_parts);

/**
 * Parse a query string into a key/value struct.
 *
 * The query string should be a null terminated string of parameters separated by
 * a delimiter. Each parameter are checked for the equal sign character. If it
 * appears in the parameter, it will be used as a null terminator and the part
 * that comes after it will be the value of the parameter.
 *
 * No data are copied, the equal sign and delimiters are used as null
 * terminators and then pointers to each parameter key and value will be stored
 * in the yuarel_param struct.
 *
 * *query:     the query string to parse. The string will be modified.
 * delimiter:  the character that separates the key/value pairs from eachother.
 * *params:    an array of (struct yuarel_param) where to store the result.
 * max_values: max number of parameters to parse.
 *
 * Returns the number of parsed items. -1 on error.
 */
extern int yuarel_parse_query(char *query, char delimiter, struct yuarel_param *params, int max_params);

#ifdef __cplusplus
}
#endif

#endif /* INC_YUAREL_H */
