
#include "http.h"


struct httpd {
  int serverSock,
      //client->sock,
      //readBufRemain,
      startTime;
  char clientAddr[HTTP_IP_ADDR_LEN],
       fileBasePath[HTTP_MAX_URL],
       *host;
  sockbuf_t client[1];
  httpReq request;
  httpRes response;
  httpVar* variables;
  httpDir* content;
  httpAcl* defaultAcl;
  FILE*  accessLog,
         *errorLog;
  void (*errorFunction304)(struct httpd* server, int error),
       (*errorFunction403)(struct httpd* server, int error),
       (*errorFunction404)(struct httpd* server, int error);
};

#define LEVEL_NOTICE  "notice"
#define LEVEL_ERROR   "error"

#define HTTP_EXPAND_TEXT  1
#define HTTP_RAW_DATA     2

char LIBHTTPD_VERSION[] = "1.3",
    LIBHTTPD_VENDOR[] = "Hughes Technologies Pty Ltd";

//#define snprintf _snprintf
//#define vsnprintf _vsnprintf



static int _httpd_net_write(int sock, const char* buf, int len)
{
  return sock_send(sock, buf, len, 0);
}
static int _httpd_net_write_str(int sock, const char* buf)
{
  int len = strlen(buf);
  return sock_send(sock, buf, len, 0);
}
static httpVar* httpdGetVariableByName(httpd* s, const char* name)
{
  httpVar* curVar;
  curVar = s->variables;
  while (curVar) {
    if (strcmp(curVar->name, name) == 0) {
      return(curVar);
    }
    curVar = curVar->nextVariable;
  }
  return(NULL);
}
static int _httpd_sendExpandedText(httpd* s, const char* buf, int bufLen)
{
  const char* textStart;
  const char* textEnd;
  const char* varStart;
  const char* varEnd;
  char varName[HTTP_MAX_VAR_NAME_LEN + 1];
  int length, offset;
  httpVar* var;
  length = offset = 0;
  textStart = buf;
  while (offset < bufLen) {
    // Look for the start of a variable name
    textEnd = strchr(textStart, '$');
    if (!textEnd) {
      // Nope. Write the remainder and bail
      _httpd_net_write(s->client->sock, textStart, bufLen - offset);
      length += bufLen - offset;
      offset += bufLen - offset;
      break;
    }
    // Looks like there could be a variable. Send the preceeding text and check it out
    _httpd_net_write(s->client->sock, textStart, textEnd - textStart);
    length += textEnd - textStart;
    offset += textEnd - textStart;
    varEnd = strchr(textEnd, '}');
    if (*(textEnd + 1) != '{' || varEnd == NULL) {
      // Nope, false alarm.
      _httpd_net_write_str(s->client->sock, "$");
      length += 1;
      offset += 1;
      textStart = textEnd + 1;
      continue;
    }
    // OK, looks like an embedded variable
    varStart = textEnd + 2;
    varEnd = varEnd - 1;
    if (varEnd - varStart > HTTP_MAX_VAR_NAME_LEN) {
      // Variable name is too long
      _httpd_net_write_str(s->client->sock, "$");
      length += 1;
      offset += 1;
      textStart = textEnd + 1;
      continue;
    }
    // Is this a known variable?
    bzero(varName, HTTP_MAX_VAR_NAME_LEN);
    strncpy(varName, varStart, varEnd - varStart + 1);
    offset += strlen(varName) + 3;
    var = httpdGetVariableByName(s, varName);
    if (!var) {
      // Nope. It's undefined. Ignore it
      textStart = varEnd + 2;
      continue;
    }
    // Write the variables value and continue
    _httpd_net_write_str(s->client->sock, var->value);
    length += strlen(var->value);
    textStart = varEnd + 2;
  }
  return(length);
}
static char* httpdRequestMethodName(httpd* s)
{
  static char tmpBuf[255];
  switch (s->request.method) {
  case HTTP_GET:
    return("GET");
  case HTTP_POST:
    return("POST");
  default:
    snprintf(tmpBuf, 255, "Invalid method '%d'",
        s->request.method);
    return(tmpBuf);
  }
}
static void _httpd_writeAccessLog(httpd* s)
{
  char dateBuf[30];
  struct tm* timePtr;
  time_t clock;
  int responseCode;
  if (s->accessLog == NULL) {
    return;
  }
  clock = time(NULL);
  timePtr = localtime(&clock);
  strftime(dateBuf, 30, "%d/%b/%Y:%T %Z", timePtr);
  responseCode = atoi(s->response.response);
  fprintf(s->accessLog, "%s - - [%s] %s \"%s\" %d %d\n",
      s->clientAddr, dateBuf, httpdRequestMethodName(s),
      httpdRequestPath(s), responseCode,
      s->response.responseLength);
}
static void _httpd_writeErrorLog(httpd* s, const char* level, const char* message)
{
  char dateBuf[30];
  struct tm* timePtr;
  time_t clock;
  if (s->errorLog == NULL) {
    return;
  }
  clock = time(NULL);
  timePtr = localtime(&clock);
  strftime(dateBuf, 30, "%a %b %d %T %Y", timePtr);
  if (*s->clientAddr != 0) {
    fprintf(s->errorLog, "[%s] [%s] [client %s] %s\n",
        dateBuf, level, s->clientAddr, message);
  }
  else {
    fprintf(s->errorLog, "[%s] [%s] %s\n",
        dateBuf, level, message);
  }
}
static int _httpd_decode(const char* bufcoded, char* bufplain, int outbufsize)
{
  static char six2pr[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
  };
  static unsigned char pr2six[256];
  // single character decode
# define DEC(c) pr2six[(int)c]
# define _DECODE_MAXVAL 63
  static int first = 1;
  int nbytesdecoded, j;
  register const char* bufin = bufcoded;
  register char* bufout = bufplain;
  register int nprbytes;
  
  // If this is the first call, initialize the mapping table. This code should work even on non-ASCII machines.
  if (first) {
    first = 0;
    for (j = 0; j < 256; j++) {
      pr2six[j] = _DECODE_MAXVAL + 1;
    }
    for (j = 0; j < 64; j++) {
      pr2six[(int)six2pr[j]] = (unsigned char)j;
    }
  }
  // Strip leading whitespace.
  while (*bufcoded == ' ' || *bufcoded == '\t') {
    bufcoded++;
  }
  
  // Figure out how many characters are in the input buffer.
  // If this would decode into more bytes than would fit into
  // the output buffer, adjust the number of input bytes downwards.
  bufin = bufcoded;
  while (pr2six[(int) * (bufin++)] <= _DECODE_MAXVAL);
  nprbytes = bufin - bufcoded - 1;
  nbytesdecoded = ((nprbytes + 3) / 4) * 3;
  if (nbytesdecoded > outbufsize) {
    nprbytes = (outbufsize * 4) / 3;
  }
  bufin = bufcoded;
  while (nprbytes > 0) {
    *(bufout++) = (unsigned char)(DEC(*bufin) << 2 | DEC(bufin[1]) >> 4);
    *(bufout++) = (unsigned char)(DEC(bufin[1]) << 4 | DEC(bufin[2]) >> 2);
    *(bufout++) = (unsigned char)(DEC(bufin[2]) << 6 | DEC(bufin[3]));
    bufin += 4;
    nprbytes -= 4;
  }
  if (nprbytes & 03) {
    if (pr2six[(int)bufin[-2]] > _DECODE_MAXVAL) {
      nbytesdecoded -= 2;
    }
    else {
      nbytesdecoded -= 1;
    }
  }
#undef DEC
#undef _DECODE_MAXVAL
  bufplain[nbytesdecoded] = 0;
  return(nbytesdecoded);
}
static char _httpd_from_hex(char c)
{
  return c >= '0' && c <= '9' ? c - '0'
      : c >= 'A' && c <= 'F' ? c - 'A' + 10
      : c >= 'a' && c <= 'f' ? c - 'a' + 10
      : 0;
  // accept small letters just in case
}
static char* _httpd_unescape(char* str)
{
  char* p = str;
  char* q = str;
  static char blank[] = "";
  if (!str) {
    return(blank);
  }
  while (*p) {
    if (*p == '%') {
      p++;
      if (*p) {
        *q = _httpd_from_hex(*p++) * 16;
      }
      if (*p) {
        *q = (*q + _httpd_from_hex(*p++));
      }
      q++;
    }
    else {
      if (*p == '+') {
        *q++ = ' ';
        p++;
      }
      else {
        *q++ = *p++;
      }
    }
  }
  *q++ = 0;
  return str;
}
static void _httpd_freeVariables(httpVar* var)
{
  httpVar* curVar, *lastVar;
  if (var == NULL) {
    return;
  }
  _httpd_freeVariables(var->nextVariable);
  var->nextVariable = NULL;
  curVar = var;
  while (curVar) {
    lastVar = curVar;
    curVar = curVar->nextValue;
    free(lastVar->name);
    free(lastVar->value);
    free(lastVar);
  }
  return;
}

static int httpdAddVariable(httpd* s, const char* name, const char* value)
{
  httpVar* curVar, *lastVar, *newVar;
  while (*name == ' ' || *name == '\t') {
    name++;
  }
  newVar = (httpVar*)malloc(sizeof(httpVar));
  bzero(newVar, sizeof(httpVar));
  newVar->name = strdup(name);
  newVar->value = strdup(value);
  lastVar = NULL;
  curVar = s->variables;
  while (curVar) {
    if (strcmp(curVar->name, name) != 0) {
      lastVar = curVar;
      curVar = curVar->nextVariable;
      continue;
    }
    while (curVar) {
      lastVar = curVar;
      curVar = curVar->nextValue;
    }
    lastVar->nextValue = newVar;
    return(0);
  }
  if (lastVar) {
    lastVar->nextVariable = newVar;
  }
  else {
    s->variables = newVar;
  }
  return(0);
}
static void _httpd_storeData(httpd* s, char* query)
{
  char* cp, *cp2, var[50], *val, *tmpVal;
  if (!query) {
    return;
  }
  cp = query;
  cp2 = var;
  bzero(var, sizeof(var));
  val = NULL;
  while (*cp) {
    if (*cp == '=') {
      cp++;
      *cp2 = 0;
      val = cp;
      continue;
    }
    if (*cp == '&') {
      *cp = 0;
      tmpVal = _httpd_unescape(val);
      httpdAddVariable(s, var, tmpVal);
      cp++;
      cp2 = var;
      val = NULL;
      continue;
    }
    if (val) {
      cp++;
    }
    else {
      *cp2 = *cp++;
      if (*cp2 == '.') {
        strcpy(cp2, "_dot_");
        cp2 += 5;
      }
      else {
        cp2++;
      }
    }
  }
  *cp = 0;
  tmpVal = _httpd_unescape(val);
  httpdAddVariable(s, var, tmpVal);
}
static void _httpd_formatTimeString(char* ptr, int len, int clock)
{
  struct tm* timePtr;
  if (clock == 0) {
    clock = time(NULL);
  }
  timePtr = gmtime((time_t*)&clock);
  if (timePtr) {
    strftime(ptr, len, "%a, %d %b %Y %T GMT", timePtr);
  }
}
static void _httpd_sendHeaders(httpd* s, int contentLength, int modTime)
{
  char tmpBuf[80],
       timeBuf[HTTP_TIME_STRING_LEN];
  if (s->response.headersSent) {
    return;
  }
  s->response.headersSent = 1;
  _httpd_net_write_str(s->client->sock, "HTTP/1.0 ");
  _httpd_net_write_str(s->client->sock, s->response.response);
  _httpd_net_write_str(s->client->sock, s->response.headers);
  _httpd_formatTimeString(timeBuf, countof(timeBuf), 0);
  _httpd_net_write_str(s->client->sock, "Date: ");
  _httpd_net_write_str(s->client->sock, timeBuf);
  _httpd_net_write_str(s->client->sock, "\n");
  _httpd_net_write_str(s->client->sock, "Connection: close\n");
  _httpd_net_write_str(s->client->sock, "Content-Type: ");
  _httpd_net_write_str(s->client->sock, s->response.contentType);
  _httpd_net_write_str(s->client->sock, "\n");
  if (contentLength > 0) {
    _httpd_net_write_str(s->client->sock, "Content-Length: ");
    snprintf(tmpBuf, sizeof(tmpBuf), "%d", contentLength);
    _httpd_net_write_str(s->client->sock, tmpBuf);
    _httpd_net_write_str(s->client->sock, "\n");
    _httpd_formatTimeString(timeBuf, countof(timeBuf), modTime);
    _httpd_net_write_str(s->client->sock, "Last-Modified: ");
    _httpd_net_write_str(s->client->sock, timeBuf);
    _httpd_net_write_str(s->client->sock, "\n");
  }
  _httpd_net_write_str(s->client->sock, "\n");
}
static httpDir* _httpd_findContentDir(httpd* s, const char* dir, int createFlag)
{
  char buffer[HTTP_MAX_URL], *curDir;
  httpDir* curItem, *curChild;
  strncpy(buffer, dir, HTTP_MAX_URL);
  curItem = s->content;
  curDir = strtok(buffer, "/");
  while (curDir) {
    curChild = curItem->children;
    while (curChild) {
      if (strcmp(curChild->name, curDir) == 0) {
        break;
      }
      curChild = curChild->next;
    }
    if (curChild == NULL) {
      if (createFlag == HTTP_TRUE) {
        curChild = (httpDir*)malloc(sizeof(httpDir));
        bzero(curChild, sizeof(httpDir));
        curChild->name = strdup(curDir);
        curChild->next = curItem->children;
        curItem->children = curChild;
      }
      else {
        return(NULL);
      }
    }
    curItem = curChild;
    curDir = strtok(NULL, "/");
  }
  return(curItem);
}
static httpContent* _httpd_findContentEntry(httpd* s, httpDir* dir, char* entryName)
{
  httpContent* curEntry;
  curEntry = dir->entries;
  while (curEntry) {
    if (curEntry->type == HTTP_WILDCARD ||
        curEntry->type == HTTP_C_WILDCARD) {
      break;
    }
    if (*entryName == 0 && curEntry->indexFlag) {
      break;
    }
    if (strcmp(curEntry->name, entryName) == 0) {
      break;
    }
    curEntry = curEntry->next;
  }
  if (curEntry) {
    s->response.content = curEntry;
  }
  return(curEntry);
}
static void httpdSetResponse(httpd* s, char* msg)
{
  strncpy(s->response.response, msg, HTTP_MAX_URL);
}
static void httpdSetContentType(httpd* s, char* type)
{
  strcpy(s->response.contentType, type);
}
static void httpdAddHeader(httpd* s, char* msg)
{
  strcat(s->response.headers, msg);
  if (msg[strlen(msg) - 1] != '\n') {
    strcat(s->response.headers, "\n");
  }
}
static void _httpd_send304(httpd* s)
{
  httpdSetResponse(s, "304 Not Modified\n");
  if (s->errorFunction304) {
    (s->errorFunction304)(s, 304);
  }
  else {
    _httpd_sendHeaders(s, 0, 0);
  }
}
static void _httpd_sendText(httpd* s, const char* msg)
{
  s->response.responseLength += strlen(msg);
  _httpd_net_write(s->client->sock, msg, strlen(msg));
}
void _httpd_send403(httpd* s)
{
  httpdSetResponse(s, "403 Permission Denied\n");
  if (s->errorFunction403) {
    (s->errorFunction403)(s, 403);
  }
  else {
    _httpd_sendHeaders(s, 0, 0);
    _httpd_sendText(s, "<HTML><HEAD><TITLE>403 Permission Denied</TITLE></HEAD>\n");
    _httpd_sendText(s, "<BODY><H1>Access to the request URL was denied!</H1>\n");
  }
}
static void _httpd_send404(httpd* s)
{
  char msg[HTTP_MAX_URL];
  snprintf(msg, HTTP_MAX_URL,
      "File does not exist: %s", s->request.path);
  _httpd_writeErrorLog(s, LEVEL_ERROR, msg);
  httpdSetResponse(s, "404 Not Found\n");
  if (s->errorFunction404) {
    (s->errorFunction404)(s, 404);
  }
  else {
    _httpd_sendHeaders(s, 0, 0);
    _httpd_sendText(s, "<HTML><HEAD><TITLE>404 Not Found</TITLE></HEAD>\n");
    _httpd_sendText(s, "<BODY><H1>The request URL was not found!</H1>\n");
    _httpd_sendText(s, "</BODY></HTML>\n");
  }
}
static void _httpd_catFile(httpd* s, const char* path, int mode)
{
  FILE* fd;
  int readLen, writeLen;
  char buf[HTTP_MAX_LEN];
  fd = fopen(path, "rb");
  if (fd == 0) {
    return;
  }
  readLen = fread(buf, HTTP_MAX_LEN - 1, 1, fd);
  while (readLen > 0) {
    if (mode == HTTP_RAW_DATA) {
      s->response.responseLength += readLen;
      _httpd_net_write(s->client->sock, buf, readLen);
    }
    else {
      buf[readLen] = 0;
      writeLen = _httpd_sendExpandedText(s, buf, readLen);
      s->response.responseLength += writeLen;
    }
    readLen = fread(buf, HTTP_MAX_LEN - 1, 1, fd);
  }
  fclose(fd);
}
static int _httpd_checkLastModified(httpd* s, int modTime)
{
  char timeBuf[HTTP_TIME_STRING_LEN];
  _httpd_formatTimeString(timeBuf, countof(timeBuf), modTime);
  if (strcmp(timeBuf, s->request.ifModified) == 0) {
    return(0);
  }
  return(1);
}
static void httpdSendHeaders(httpd* s)
{
  _httpd_sendHeaders(s, 0, 0);
}
static void httpdOutput(httpd* s, char* msg)
{
  char buf[HTTP_MAX_LEN],
       varName[80],
       *src,
       *dest;
  int count;
  src = msg;
  dest = buf;
  count = 0;
  while (*src && count < HTTP_MAX_LEN) {
    if (*src == '$') {
      char* cp,
            *tmp;
      int count2;
      httpVar* curVar;
      tmp = src + 1;
      cp = varName;
      count2 = 0;
      while (*tmp && (isalnum(*tmp) || *tmp == '_') && count2 < 80) {
        *cp++ = *tmp++;
        count2++;
      }
      *cp = 0;
      curVar = httpdGetVariableByName(s, varName);
      if (curVar) {
        strcpy(dest, curVar->value);
        dest = dest + strlen(dest);
        count += strlen(dest);
      }
      else {
        *dest++ = '$';
        strcpy(dest, varName);
        dest += strlen(varName);
        count += 1 + strlen(varName);
      }
      src = src + strlen(varName) + 1;
      continue;
    }
    *dest++ = *src++;
    count++;
  }
  *dest = 0;
  s->response.responseLength += strlen(buf);
  if (s->response.headersSent == 0) {
    httpdSendHeaders(s);
  }
  _httpd_net_write_str(s->client->sock, buf);
}
static void _httpd_sendStatic(httpd* s, char* data)
{
  if (_httpd_checkLastModified(s, s->startTime) == 0) {
    _httpd_send304(s);
  }
  _httpd_sendHeaders(s, s->startTime, strlen(data));
  httpdOutput(s, data);
}
static void _httpd_sendFile(httpd* s, char* path)
{
  char* suffix;
  sys_stat sbuf[1];
  suffix = strrchr(path, '.');
  if (suffix != NULL) {
    if (strcasecmp(suffix, ".gif") == 0) {
      strcpy(s->response.contentType, "image/gif");
    }
    if (strcasecmp(suffix, ".jpg") == 0) {
      strcpy(s->response.contentType, "image/jpeg");
    }
    if (strcasecmp(suffix, ".xbm") == 0) {
      strcpy(s->response.contentType, "image/xbm");
    }
    if (strcasecmp(suffix, ".png") == 0) {
      strcpy(s->response.contentType, "image/png");
    }
    if (strcasecmp(suffix, ".css") == 0) {
      strcpy(s->response.contentType, "text/css");
    }
  }
  if (sys_filestat(path, sbuf) < 0) {
    _httpd_send404(s);
    return;
  }
  if (_httpd_checkLastModified(s, sbuf->mtime) == 0) {
    _httpd_send304(s);
  }
  else {
    _httpd_sendHeaders(s, sbuf->size, sbuf->mtime);
    if (strncmp(s->response.contentType, "text/", 5) == 0) {
      _httpd_catFile(s, path, HTTP_EXPAND_TEXT);
    }
    else {
      _httpd_catFile(s, path, HTTP_RAW_DATA);
    }
  }
}
static int _httpd_sendDirectoryEntry(httpd* s, httpContent* entry, char* entryName)
{
  char path[HTTP_MAX_URL];
  snprintf(path, HTTP_MAX_URL, "%s/%s", entry->path, entryName);
  _httpd_sendFile(s, path);
  return(0);
}
static char* _httpd_escape(const char* str)
{
// Overencodes
#define URL_XALPHAS  (unsigned char) 1
#define URL_XPALPHAS (unsigned char) 2
// Bit 0 xalpha -- see HTFile.h
// Bit 1 xpalpha -- as xalpha but with plus.
// Bit 2 ... path -- as xpalpha but with /
// 0 1 2 3 4 5 6 7 8 9 A B C D E F
  static unsigned char isAcceptable[96] =
  {
    7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 7, 7, 7, /* 2x !"#$%&'()*+,-./ */
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, /* 3x 0123456789:;<=>? */
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, /* 4x @ABCDEFGHIJKLMNO */
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0, 7, /* 5X PQRSTUVWXYZ[\]^_ */
      0, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, /* 6x `abcdefghijklmno */
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0
  }; /* 7X pqrstuvwxyz{\}~ DEL */
#define ACCEPTABLE(a) ( a>=32 && a<128 && ((isAcceptable[a-32]) & mask))
  static char* hex = "0123456789ABCDEF";
  unsigned char mask = URL_XPALPHAS;
  const char *p;
  char *q, *result;
  int unacceptable = 0;
  for (p = str; *p; p++) {
    if (!ACCEPTABLE((unsigned char)*p)) {
      unacceptable += 2;
    }
  }
  result = (char*) malloc(p - str + unacceptable + 1);
  bzero(result, (p - str + unacceptable + 1));
  if (result == NULL) {
    return(NULL);
  }
  for (q = result, p = str; *p; p++) {
    unsigned char a = *p;
    if (!ACCEPTABLE(a)) {
      *q++ = '%'; // Means hex commming
      *q++ = hex[a >> 4];
      *q++ = hex[a & 15];
    }
    else {
      *q++ = *p;
    }
  }
  *q++ = 0; // Terminate
  return result;
}
#undef URL_XALPHAS
#undef URL_XPALPHAS
static void _httpd_sanitiseUrl(char* url)
{
  char* from, *to, *last;
  // Remove multiple slashes
  from = to = url;
  while (*from) {
    if (*from == '/' && *(from + 1) == '/') {
      from++;
      continue;
    }
    *to = *from;
    to++;
    from++;
  }
  *to = 0;
  // Get rid of ./ sequences
  from = to = url;
  while (*from) {
    if (*from == '/' && *(from + 1) == '.' && *(from + 2) == '/') {
      from += 2;
      continue;
    }
    *to = *from;
    to++;
    from++;
  }
  *to = 0;
  // Catch use of /../ sequences and remove them. Must track the
  // path structure and remove the previous path element.
  from = to = last = url;
  while (*from) {
    if (*from == '/' && *(from + 1) == '.' &&
        *(from + 2) == '.' && *(from + 3) == '/') {
      to = last;
      from += 3;
      continue;
    }
    if (*from == '/') {
      last = to;
    }
    *to = *from;
    to++;
    from++;
  }
  *to = 0;
}
static int scanCidr(const char* val, int* result, int* length)
{
  int res, res1, res2, res3, res4, res5;
  const char* cp;
  cp = val;
  res1 = atoi(cp);
  cp = strchr(cp, '.');
  if (!cp) {
    return(-1);
  }
  cp++;
  res2 = atoi(cp);
  cp = strchr(cp, '.');
  if (!cp) {
    return(-1);
  }
  cp++;
  res3 = atoi(cp);
  cp = strchr(cp, '.');
  if (!cp) {
    return(-1);
  }
  cp++;
  res4 = atoi(cp);
  cp = strchr(cp, '/');
  if (!cp) {
    res5 = 32;
  }
  else {
    cp++;
    res5 = atoi(cp);
  }
  if (res1 > 255 || res2 > 255 || res3 > 255 || res4 > 255 || res5 > 32) {
    return(-1);
  }
  res = (res1 << 24) + (res2 << 16) + (res3 << 8) + res4;
  *result = res;
  *length = res5;
  return(0);
}
static int _isInCidrBlock(httpd* s, int addr1, int len1, int addr2, int len2)
{
  int count, mask;
  /* if (addr1 == 0 && len1 == 0)  {  return(1);  } */
  if (len2 < len1) {
    _httpd_writeErrorLog(s, LEVEL_ERROR,
        "IP Address must be more specific than network block");
    return(0);
  }
  mask = count = 0;
  while (count < len1) {
    mask = (mask << 1) + 1;
    count++;
  }
  mask = mask << (32 - len1);
  if ((addr1 & mask) == (addr2 & mask)) {
    return(1);
  }
  else {
    return(0);
  }
}
static httpAcl* httpdAddAcl(httpd* s, httpAcl* acl, char* cidr, int action)
{
  httpAcl* cur;
  int addr, len;
  // Check the ACL info is reasonable
  if (scanCidr(cidr, &addr, &len) < 0) {
    _httpd_writeErrorLog(s, LEVEL_ERROR, "Invalid IP address format");
    return(NULL);
  }
  if (action != HTTP_ACL_PERMIT && action != HTTP_ACL_DENY) {
    _httpd_writeErrorLog(s, LEVEL_ERROR, "Invalid acl action");
    return(NULL);
  }
  // Find a spot to put this ACE
  if (acl) {
    cur = acl;
    while (cur->next) {
      cur = cur->next;
    }
    cur->next = (httpAcl*)malloc(sizeof(httpAcl));
    cur = cur->next;
  }
  else {
    cur = (httpAcl*)malloc(sizeof(httpAcl));
    acl = cur;
  }
  // Add the details and return
  cur->addr = addr;
  cur->len = len;
  cur->action = action;
  cur->next = NULL;
  return(acl);
}
static int httpdCheckAcl(httpd* s, httpAcl* acl)
{
  httpAcl* cur;
  int addr, len, res, action;
  action = HTTP_ACL_DENY;
  scanCidr(s->clientAddr, &addr, &len);
  cur = acl;
  while (cur) {
    res = _isInCidrBlock(s, cur->addr, cur->len, addr, len);
    if (res == 1) {
      action = cur->action;
      break;
    }
    cur = cur->next;
  }
  if (action == HTTP_ACL_DENY) {
    _httpd_send403(s);
    _httpd_writeErrorLog(s, LEVEL_ERROR, "Access denied by ACL");
  }
  return(action);
}
//static void httpdSetDefaultAcl(httpd* s, httpAcl* acl){  s->defaultAcl = acl; }
static char* httpdUrlEncode(const char* str)
{
  char* s, *cp;
  s = (char*)_httpd_escape(str);
  if (s == NULL) {
    return(NULL);
  }
  cp = s;
  while (*cp) {
    if (*cp == ' ') {
      *cp = '+';
    }
    cp++;
  }
  return(s);
}
static httpVar* httpdGetVariableByPrefix(httpd* s, char* prefix)
{
  httpVar* curVar;
  if (prefix == NULL) {
    return(s->variables);
  }
  curVar = s->variables;
  while (curVar) {
    if (strncmp(curVar->name, prefix, strlen(prefix)) == 0) {
      return(curVar);
    }
    curVar = curVar->nextVariable;
  }
  return(NULL);
}
static httpVar* httpdGetVariableByPrefixedName(httpd* s, char* prefix, char* name)
{
  httpVar* curVar;
  int prefixLen;
  if (prefix == NULL) {
    return(s->variables);
  }
  curVar = s->variables;
  prefixLen = strlen(prefix);
  while (curVar) {
    if (strncmp(curVar->name, prefix, prefixLen) == 0 &&
        strcmp(curVar->name + prefixLen, name) == 0) {
      return(curVar);
    }
    curVar = curVar->nextVariable;
  }
  return(NULL);
}
static httpVar* httpdGetNextVariableByPrefix(httpVar* curVar, char* prefix)
{
  if (curVar) {
    curVar = curVar->nextVariable;
  }
  while (curVar) {
    if (strncmp(curVar->name, prefix, strlen(prefix)) == 0) {
      return(curVar);
    }
    curVar = curVar->nextVariable;
  }
  return(NULL);
}
static int httpdSetVariableValue(httpd* s, char* name, char* value)
{
  httpVar* var;
  var = httpdGetVariableByName(s, name);
  if (var) {
    if (var->value) {
      free(var->value);
    }
    var->value = strdup(value);
    return(0);
  }
  else {
    return(httpdAddVariable(s, name, value));
  }
}
static httpd* httpdCreate(const char* host, int port)
{
  httpd* s;
  int sock;
  addr_in addr[1] = {0};
  // Create the handle and setup it's basic config
  s = (httpd*)malloc(sizeof(httpd));
  if (s == NULL) {
    return(NULL);
  }
  bzero(s, sizeof(httpd));
  //port;
  if (host == NULL) {
    s->host = HTTP_ANY_ADDR;
  }
  else {
    s->host = strdup(host);
  }
  s->content = (httpDir*)malloc(sizeof(httpDir));
  bzero(s->content, sizeof(httpDir));
  s->content->name = strdup("");
  
  // Setup the socket
  sock = sock_open(s->host, port, SOCK_TCP, addr);
  if (sock < 0) {
    free(s);
    return(NULL);
  }
#ifdef SO_REUSEADDR
  opt = 1;
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(int));
#endif
  s->serverSock = sock;
  if (sock_bind(sock, addr) < 0) {
    sock_close(sock);
    free(s);
    return(NULL);
  }
  sock_listen(sock, 128);
  s->startTime = time(NULL);
  return(s);
}
static void httpdDestroy(httpd* s)
{
  if (s == NULL) {
    return;
  }
  if (s->host) {
    free(s->host);
  }
  free(s);
}
static void httpdEndRequest(httpd* s)
{
  _httpd_freeVariables(s->variables);
  s->variables = NULL;
  sock_shutdown(s->client->sock, 2);
  sock_close(s->client->sock);
  if (s->request.content) {
    free(s->request.content);
  }
  bzero(&s->request, sizeof(s->request));
}
static int httpdGetConnection(httpd* s, int time_ms)
{
  int result;
  sock_set fds[1] = {0};
  addr_in addr[1] = {0};
  timeval_t timeout[1];
  timeout->tv_sec = time_ms/1000;
  timeout->tv_usec = time_ms%1000;
  sock_set_add(s->serverSock, fds);
  result = 0;
  while (result == 0) {
    result = sock_select(s->serverSock + 1, fds, 0, 0, timeout);
    if (result < 0) {
      return(-1);
    }
    if (timeout != 0 && result == 0) {
      return(0);
    }
    if (result > 0) {
      break;
    }
  }
  s->client->sock = sock_accept(s->serverSock, addr);
  addr_tostr(addr, s->clientAddr, countof(s->clientAddr));
  s->client->readBufRemain = 0;
  s->client->readBufPtr = NULL;
  // Check the default ACL
  if (s->defaultAcl) {
    if (httpdCheckAcl(s, s->defaultAcl) == HTTP_ACL_DENY) {
      httpdEndRequest(s);
      return(-2);
    }
  }
  return(1);
}

static int httpdReadRequest(httpd* s)
{
  static char buf[HTTP_MAX_LEN];
  int count, inHeaders;
  char* cp, *cp2;
  // Setup for a standard response
  strcpy(s->response.headers, "Server: Hughes Technologies Embedded Server\n");
  strcpy(s->response.contentType, "text/html");
  strcpy(s->response.response, "200 Output Follows\n");
  s->response.headersSent = 0;
  // Read the request
  count = 0;
  inHeaders = 1;
  while (sockbuf_readLine(s->client, buf, HTTP_MAX_LEN) > 0) {
    count++;
    printf("%s\n", buf);
    // Special case for the first line. Scan the request method and path etc
    if (count == 1) {
      //First line. Scan the request info
      cp = cp2 = buf;
      while (isalpha(*cp2)) {
        cp2++;
      }
      *cp2 = 0;
      if (strcasecmp(cp, "GET") == 0) {
        s->request.method = HTTP_GET;
      } else if (strcasecmp(cp, "POST") == 0) {
        s->request.method = HTTP_POST;
      } else {
        printf("err\n");
      }
      if (s->request.method == 0) {
        _httpd_net_write_str(s->client->sock, HTTP_METHOD_ERROR);
        _httpd_net_write_str(s->client->sock, cp);
        _httpd_writeErrorLog(s, LEVEL_ERROR, "Invalid method received");
        return(-1);
      }
      cp = cp2 + 1;
      while (*cp == ' ') {
        cp++;
      }
      cp2 = cp;
      while (*cp2 != ' ' && *cp2 != 0) {
        cp2++;
      }
      *cp2 = 0;
      strncpy(s->request.path, cp, HTTP_MAX_URL);
      _httpd_sanitiseUrl(s->request.path);
      continue;
    }
    // Process the headers
    if (inHeaders) {
      if (*buf == 0) {
        // End of headers. Continue if there's data to read
        if (s->request.contentLength == 0) {
          break;
        }
        inHeaders = 0;
        break;
      } else if (STRNCASECMP2(buf, "Cookie:") == 0) {
        char* var, *val, *end;
        var = strchr(buf, ':');
        while (var) {
          var++;
          val = strchr(var, '=');
          *val = 0;
          val++;
          end = strchr(val, ';');
          if (end) {
            *end = 0;
          }
          httpdAddVariable(s, var, val);
          var = end;
        }
      } else if (STRNCASECMP2(buf, "Authorization:") == 0) {
        cp = strchr(buf, ':') + 2;
        if (strncmp(cp, "Basic ", 6) != 0) {
          // Unknown auth method
        }
        else {
          char authBuf[100];
          cp = strchr(cp, ' ') + 1;
          _httpd_decode(cp, authBuf, 100);
          s->request.authLength = strlen(authBuf);
          cp = strchr(authBuf, ':');
          if (cp) {
            *cp = 0;
            strncpy(s->request.authPassword, cp + 1, HTTP_MAX_AUTH);
          }
          strncpy(s->request.authUser, authBuf, HTTP_MAX_AUTH);
        }
      } else if (STRNCASECMP2(buf, "Host:") == 0) {
        cp = strchr(buf, ':') + 2;
        if (cp) {
          strncpy(s->request.host, cp, HTTP_MAX_URL);
        }
      } else if (STRNCASECMP2(buf, "Referer:") == 0) {
        cp = strchr(buf, ':') + 2;
        if (cp) {
          strncpy(s->request.referer, cp, HTTP_MAX_URL);
        }
      } else if (STRNCASECMP2(buf, "If-Modified-Since:") == 0) {
        cp = strchr(buf, ':') + 2;
        if (cp) {
          strncpy(s->request.ifModified, cp, HTTP_MAX_URL);
          cp = strchr(s->request.ifModified, ';');
          if (cp) {
            *cp = 0;
          }
        }
      } else if (STRNCASECMP2(buf, "Content-Type:") == 0) {
        //char *cp1, *cp2;
        cp = strchr(buf, ':');
        if (cp) {
          cp += 1;
          for (; *cp==' '; ++cp);
          strncpy(s->request.contentType, cp, HTTP_MAX_URL);
        }
      } else if (STRNCASECMP2(buf, "Content-Length:") == 0) {
        cp = strchr(buf, ':') + 2;
        if (cp) {
          s->request.contentLength = atoi(cp);
        }
      } else if (STRNCASECMP2(buf, "User-Agent:") == 0) {
        //printf("User-Agent: %s\n", cp);
      } else if (STRNCASECMP2(buf, "Accept-Encoding:") == 0) {
        //printf("Accept-Encoding: %s\n", cp);
      } else if (STRNCASECMP2(buf, "Accept:") == 0) {
        //printf("Accept-Encoding: %s\n", cp);
      } else if (STRNCASECMP2(buf, "Connection:") == 0) {
      } else if (STRNCASECMP2(buf, "Cache-Control") == 0) {
      } else if (STRNCASECMP2(buf, "Upgrade-Insecure-Requests") == 0) {
      } else if (STRNCASECMP2(buf, "Accept-Language") == 0) {
      } else {
        //ASSERT(0);
      }
      continue;
    }
  }
  // Process and POST data
  if (s->request.contentLength > 0) {
    //bzero(buf, HTTP_MAX_LEN);
    s->request.content = (char*)malloc(s->request.contentLength+1);
    sockbuf_readBuf(s->client, s->request.content, s->request.contentLength);
    s->request.content[s->request.contentLength]=0;
    //savefile("D://aaa.txt", s->request.content, s->request.contentLength);
    //_httpd_storeData(s, buf);
  }
  // Process any URL data
  cp = strchr(s->request.path, '?');
  if (cp != NULL) {
    *cp = 0;
    cp++;
    _httpd_storeData(s, cp);
  }
  return(0);
}
static void httpdFreeVariables(httpd* s)
{
  _httpd_freeVariables(s->variables);
}
static void httpdDumpVariables(httpd* s)
{
  httpVar* curVar, *curVal;
  curVar = s->variables;
  while (curVar) {
    printf("Variable '%s'\n", curVar->name);
    curVal = curVar;
    while (curVal) {
      printf("\t= '%s'\n", curVal->value);
      curVal = curVal->nextValue;
    }
    curVar = curVar->nextVariable;
  }
}
static void httpdSetFileBase(httpd* s, char* path)
{
  strncpy(s->fileBasePath, path, HTTP_MAX_URL);
}
static int httpdAddFileContent(httpd* s, char* dir, char* name, int indexFlag, int (*preload)(httpd* s), char* path)
{
  httpDir* dirPtr;
  httpContent* newEntry;
  dirPtr = _httpd_findContentDir(s, dir, HTTP_TRUE);
  newEntry = (httpContent*)malloc(sizeof(httpContent));
  if (newEntry == NULL) {
    return(-1);
  }
  bzero(newEntry, sizeof(httpContent));
  newEntry->name = strdup(name);
  newEntry->type = HTTP_FILE;
  newEntry->indexFlag = indexFlag;
  newEntry->preload = preload;
  newEntry->next = dirPtr->entries;
  dirPtr->entries = newEntry;
  if (*path == '/') {
    // Absolute path
    newEntry->path = strdup(path);
  }
  else {
    // Path relative to base path
    newEntry->path = (char*)malloc(strlen(s->fileBasePath) + strlen(path) + 2);
    snprintf(newEntry->path, HTTP_MAX_URL, "%s/%s",
        s->fileBasePath, path);
  }
  return(0);
}
static int httpdAddWildcardContent(httpd* s, char* dir, int (*preload)(httpd* s), char* path)
{
  httpDir* dirPtr;
  httpContent* newEntry;
  dirPtr = _httpd_findContentDir(s, dir, HTTP_TRUE);
  newEntry = (httpContent*)malloc(sizeof(httpContent));
  if (newEntry == NULL) {
    return(-1);
  }
  bzero(newEntry, sizeof(httpContent));
  newEntry->name = NULL;
  newEntry->type = HTTP_WILDCARD;
  newEntry->indexFlag = HTTP_FALSE;
  newEntry->preload = preload;
  newEntry->next = dirPtr->entries;
  dirPtr->entries = newEntry;
  if (*path == '/') {
    // Absolute path
    newEntry->path = strdup(path);
  }
  else {
    // Path relative to base path
    newEntry->path = (char*)malloc(strlen(s->fileBasePath) + strlen(path) + 2);
    snprintf(newEntry->path, HTTP_MAX_URL, "%s/%s", s->fileBasePath, path);
  }
  return(0);
}
static int httpdAddCContent(httpd* s, const char* dir, const char* name, int indexFlag, int (*preload)(httpd* s), void (*function)(httpd* s))
{
  httpDir* dirPtr;
  httpContent* newEntry;
  dirPtr = _httpd_findContentDir(s, dir, HTTP_TRUE);
  newEntry = (httpContent*)malloc(sizeof(httpContent));
  if (newEntry == NULL) {
    return(-1);
  }
  bzero(newEntry, sizeof(httpContent));
  newEntry->name = strdup(name);
  newEntry->type = HTTP_C_FUNCT;
  newEntry->indexFlag = indexFlag;
  newEntry->function = function;
  newEntry->preload = preload;
  newEntry->next = dirPtr->entries;
  dirPtr->entries = newEntry;
  return(0);
}
static int httpdAddCWildcardContent(httpd* s, const char* dir, int (*preload)(httpd* s), void (*function)(httpd* s))
{
  httpDir* dirPtr;
  httpContent* newEntry;
  dirPtr = _httpd_findContentDir(s, dir, HTTP_TRUE);
  newEntry = (httpContent*)malloc(sizeof(httpContent));
  if (newEntry == NULL) {
    return(-1);
  }
  bzero(newEntry, sizeof(httpContent));
  newEntry->name = NULL;
  newEntry->type = HTTP_C_WILDCARD;
  newEntry->indexFlag = HTTP_FALSE;
  newEntry->function = function;
  newEntry->preload = preload;
  newEntry->next = dirPtr->entries;
  dirPtr->entries = newEntry;
  return(0);
}
static int httpdAddStaticContent(httpd* s, char* dir, char* name, int indexFlag, int (*preload)(httpd* s), char* data)
{
  httpDir* dirPtr;
  httpContent* newEntry;
  dirPtr = _httpd_findContentDir(s, dir, HTTP_TRUE);
  newEntry = (httpContent*)malloc(sizeof(httpContent));
  if (newEntry == NULL) {
    return(-1);
  }
  bzero(newEntry, sizeof(httpContent));
  newEntry->name = strdup(name);
  newEntry->type = HTTP_STATIC;
  newEntry->indexFlag = indexFlag;
  newEntry->data = data;
  newEntry->preload = preload;
  newEntry->next = dirPtr->entries;
  dirPtr->entries = newEntry;
  return(0);
}
static int httpdAddEmberContect(httpd* s, char* dir, char* name, int indexFlag, int (*preload)(httpd* s), char* script)
{
  httpDir* dirPtr;
  httpContent* newEntry;
  dirPtr = _httpd_findContentDir(s, dir, HTTP_TRUE);
  newEntry = (httpContent*)malloc(sizeof(httpContent));
  if (newEntry == NULL) {
    return(-1);
  }
  bzero(newEntry, sizeof(httpContent));
  newEntry->name = strdup(name);
  newEntry->type = HTTP_EMBER_FUNCT;
  newEntry->indexFlag = indexFlag;
  newEntry->data = script;
  newEntry->preload = preload;
  newEntry->next = dirPtr->entries;
  dirPtr->entries = newEntry;
  return(0);
}
static void httpdSetCookie(httpd* s, char* name, char* value)
{
  char buf[HTTP_MAX_URL];
  snprintf(buf, HTTP_MAX_URL, "Set-Cookie: %s=%s; path=/;", name, value);
  httpdAddHeader(s, buf);
}
static void httpdPrintf(httpd* s, char* fmt, ...)
{
  va_list args;
  char buf[HTTP_MAX_LEN];
  va_start(args, fmt);
  if (s->response.headersSent == 0) {
    httpdSendHeaders(s);
  }
  vsnprintf(buf, HTTP_MAX_LEN, fmt, args);
  s->response.responseLength += strlen(buf);
  _httpd_net_write_str(s->client->sock, buf);
  printf("%s\n", buf);
}
static void httpdSendFile(httpd* s, char* path)
{
  char* suffix;
  sys_stat sbuf[1];
  suffix = strrchr(path, '.');
  if (suffix != NULL) {
    if (strcasecmp(suffix, ".gif") == 0) {
      strcpy(s->response.contentType, "image/gif");
    }
    if (strcasecmp(suffix, ".jpg") == 0) {
      strcpy(s->response.contentType, "image/jpeg");
    }
    if (strcasecmp(suffix, ".xbm") == 0) {
      strcpy(s->response.contentType, "image/xbm");
    }
    if (strcasecmp(suffix, ".png") == 0) {
      strcpy(s->response.contentType, "image/png");
    }
    if (strcasecmp(suffix, ".css") == 0) {
      strcpy(s->response.contentType, "text/css");
    }
  }
  if (sys_filestat(path, sbuf) < 0) {
    _httpd_send404(s);
    return;
  }
  if (_httpd_checkLastModified(s, sbuf->mtime) == 0) {
    _httpd_send304(s);
  }
  else {
    _httpd_sendHeaders(s, sbuf->size, sbuf->mtime);
    if (strncmp(s->response.contentType, "text/", 5) == 0) {
      _httpd_catFile(s, path, HTTP_EXPAND_TEXT);
    }
    else {
      _httpd_catFile(s, path, HTTP_RAW_DATA);
    }
  }
}
static void httpdProcessRequest(httpd* s)
{
  char dirName[HTTP_MAX_URL], entryName[HTTP_MAX_URL], *cp;
  httpDir* dir;
  httpContent* entry;
  s->response.responseLength = 0;
  strncpy(dirName, httpdRequestPath(s), HTTP_MAX_URL);
  cp = strrchr(dirName, '/');
  if (cp == NULL) {
    printf("Invalid request path '%s'\n", dirName);
    return;
  }
  strncpy(entryName, cp + 1, HTTP_MAX_URL);
  if (cp != dirName) {
    *cp = 0;
  }
  else {
    *(cp + 1) = 0;
  }

  dir = _httpd_findContentDir(s, dirName, HTTP_FALSE);
  if (dir == NULL) {
    _httpd_send404(s);
    _httpd_writeAccessLog(s);
    return;
  }
  entry = _httpd_findContentEntry(s, dir, entryName);
  if (entry == NULL) {
    _httpd_send404(s);
    _httpd_writeAccessLog(s);
    return;
  }
  if (entry->preload) {
    if ((entry->preload)(s) < 0) {
      _httpd_writeAccessLog(s);
      return;
    }
  }
  switch (entry->type) {
  case HTTP_C_FUNCT:
  case HTTP_C_WILDCARD:
    (entry->function)(s);
    break;
#ifdef HAVE_EMBER
  case HTTP_EMBER_FUNCT:
  case HTTP_EMBER_WILDCARD:
    _httpd_executeEmber(s, entry->data);
    break;
#endif
  case HTTP_STATIC:
    _httpd_sendStatic(s, entry->data);
    break;
  case HTTP_FILE:
    httpdSendFile(s, entry->path);
    break;
  case HTTP_WILDCARD:
    if (_httpd_sendDirectoryEntry(s, entry, entryName) < 0) {
      _httpd_send404(s);
    }
    break;
  }
  _httpd_writeAccessLog(s);
}
static void httpdSetAccessLog(httpd* s, FILE* fp)
{
  s->accessLog = fp;
}
static void httpdSetErrorLog(httpd* s, FILE* fp)
{
  s->errorLog = fp;
}
static int httpdAuthenticate(httpd* s, char* realm)
{
  char buffer[255];
  if (s->request.authLength == 0) {
    httpdSetResponse(s, "401 Please Authenticate");
    snprintf(buffer, sizeof(buffer), "WWW-Authenticate: Basic realm=\"%s\"\n", realm);
    httpdAddHeader(s, buffer);
    httpdOutput(s, "\n");
    return(0);
  }
  return(1);
}
static void httpdForceAuthenticate(httpd* s, char* realm)
{
  char buffer[255];
  httpdSetResponse(s, "401 Please Authenticate");
  snprintf(buffer, sizeof(buffer), "WWW-Authenticate: Basic realm=\"%s\"\n", realm);
  httpdAddHeader(s, buffer);
  httpdOutput(s, "\n");
}
static int httpdSetErrorFunction(httpd* s, int error, void (*function)(httpd* s, int error))
{
  static char errBuf[80];
  switch (error) {
  case 304:
    s->errorFunction304 = function;
    break;
  case 403:
    s->errorFunction403 = function;
    break;
  case 404:
    s->errorFunction404 = function;
    break;
  default:
    snprintf(errBuf, 80, "Invalid error code (%d) for custom callback", error);
    _httpd_writeErrorLog(s, LEVEL_ERROR, errBuf);
    return(-1);
    break;
  }
  return(0);
}
