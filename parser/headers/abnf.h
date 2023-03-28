#define ABNF_H
typedef struct abnf node;
struct abnf {
   char *label; //a def dans le main
   char *debut;
   char *fin;
   node *fils;
   node *frere;
};

extern char HTTP_MESSAGE[];
extern char REQUEST_LINE[];
extern char START_LINE[];
extern char METHOD[];
extern char TOKEN[];
extern char TCHAR[];
extern char SP[];
extern char ALPHA[];
extern char DIGIT[];
extern char REQUEST_TARGET[];
extern char ABSOLUTE_PATH[];
extern char SEGMENT[];
extern char PCHAR[];
extern char UNRESERVED[];
extern char PCT_ENCODED[];
extern char SUB_DELIMS[];
extern char HEXDIG[];
extern char QUERY[];
extern char HTTP_VERSION[];
extern char HTTP_NAME[];
extern char CRLF[];
extern char HEADER_FIELD[];
extern char ICAR[];
extern char FIELD_NAME[];
extern char FIELD_VALUE[];
extern char OWS[];
extern char HTAB[];
extern char FIELD_CONTENT[];
extern char OBS_FOLD[];
extern char FIELD_VCHAR[];
extern char VCHAR[];
extern char OBS_TEXT[];
extern char MESSAGE_BODY[];
extern char OCTET[];
extern char CONNECTION[];
extern char HOST[];
extern char CONTENT_LENGTH[];
extern char CONTENT_TYPE[];
extern char COOKIE[];
extern char TRANSFER_ENCODING[];
extern char EXPECT[];
extern char CONNECTION_OPTION[];
extern char URI_HOST[];
extern char PORT[];
extern char IPV4ADDRESS[];
extern char IP_LITERAL[];
extern char REG_NAME[];
extern char HOST_LOWER[];
extern char IPV6ADDRESS[];
extern char IPVFUTURE[];
extern char DEC_OCTET[];
extern char H16[];
extern char LS32[];

extern node* racine;

void http_message(char **current_char, node *struct_current);
void icar(char** current_char, node *new_struct_2);
void request_line(char **current_char, node *struct_current);
void start_line(char **current_char, node *struct_current);
void method(char **current_char, node *struct_current);
void token(char **current_char, node *struct_current);
void request_target(char **current_char, node *struct_current);
void absolute_path(char **current_char, node *struct_current);
void segment(char **current_char, node *struct_current);
void pchar(char **current_char, node *struct_current);
void tchar(char **current_char, node *struct_current);
void alpha(char **current_char, node *struct_current);
void digit(char **current_char, node *struct_current);
void sp(char **current_char, node *struct_current);void query(char **current_char, node *struct_current);
void sub_delims(char **current_char, node *struct_current);
void unreserved(char **current_char, node *struct_current);
void pct_encoded(char **current_char, node *struct_current);
void hexdig(char **current_char, node *struct_current);
void crlf(char **current_char, node *struct_current);
void http_name(char **current_char, node *struct_current);
void http_version(char **current_char, node *struct_current);
void field_name(char **current_char, node *struct_current);
void ows(char **current_char, node *struct_current);
void htab(char **current_char, node *struct_current);
void field_content(char **current_char, node *struct_current);
void obs_fold(char **current_char, node *struct_current);
void field_vchar(char **current_char, node *struct_current);
void field_value(char **current_char, node *struct_current);
void vchar(char **current_char, node *struct_current);
void obs_text(char **current_char, node *struct_current);
void message_body(char **current_char, node *struct_current);
void octet(char **current_char, node *struct_current);
void header_field(char **current_char, node *struct_current);
void connection_header(char **current_char, node *struct_current);
void host_header(char **current_char, node *struct_current);
void content_length_header(char **current_char, node *struct_current);
void content_type_header(char **current_char, node *struct_current);
void cookie_header(char **current_char, node *struct_current);
void transfer_encoding_header(char **current_char, node *struct_current);
void expect_header(char **current_char, node *struct_current);
void connection_option(char **current_char, node *struct_current);
void uri_host(char **current_char, node *struct_current);
void host(char **current_char, node *struct_current);
void port(char **current_char, node *struct_current);
void ipv4address(char **current_char, node *struct_current);
void ip_literal(char **current_char, node *struct_current);
void reg_name(char **current_char, node *struct_current);
void dec_octet(char **current_char, node *struct_current);
void host_port(char** current_char, node* struct_current);
int isipv4address(char* current_char);
void ipv6address(char **current_char, node *struct_current);
int issubdelims(char c);
int subdelims(char** current_char, node* struct_current);
void reg_name(char** current_char, node* struct_current);