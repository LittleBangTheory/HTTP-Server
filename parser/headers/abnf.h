#define ABNF_H
typedef struct abnf node;
struct abnf {
   char *label; //a def dans le main
   unsigned char *debut;
   unsigned char *fin;
   node *fils;
   node *frere;
};
extern node** racine;
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
extern char IPV6_ADDRESS[];
extern char IPVFUTURE[];
extern char DEC_OCTET[];
extern char H16[];
extern char LS32[];
extern char REGNAME[];
extern char MEDIA_TYPE[];
extern char TYPE[];
extern char SUBTYPE[];
extern char PARAMETER[];
extern char QUOTED_STRING[];
extern char QDTEXT[];
extern char QUOTED_PAIR[];
extern char COOKIE_STRING[];
extern char COOKIE_PAIR[];
extern char COOKIE_NAME[];
extern char COOKIE_VALUE[];
extern char COOKIE_OCTET[];
extern char TRANSFER_ENCODING_HEADER[];
extern char EXPECT_HEADER[];
extern char ISTRING[];
extern char TRANSFER_CODING[];
extern char TRANSFER_EXTENSION[];
extern char TRANSFER_PARAMETER[];


void http_message(unsigned char **current_char, node *struct_current);
void icar(unsigned char** current_char, node *new_struct_2);
void request_line(unsigned char **current_char, node *struct_current);
void start_line(unsigned char **current_char, node *struct_current);
void method(unsigned char **current_char, node *struct_current);
void token(unsigned char **current_char, node *struct_current);
void request_target(unsigned char **current_char, node *struct_current);
void absolute_path(unsigned char **current_char, node *struct_current);
void segment(unsigned char **current_char, node *struct_current);
void pchar(unsigned char **current_char, node *struct_current);
void tchar(unsigned char **current_char, node *struct_current);
void alpha(unsigned char **current_char, node *struct_current);
void digit(unsigned char **current_char, node *struct_current);
void sp(unsigned char **current_char, node *struct_current);
void query(unsigned char **current_char, node *struct_current);
void sub_delims(unsigned char **current_char, node *struct_current);
void unreserved(unsigned char **current_char, node *struct_current);
void pct_encoded(unsigned char **current_char, node *struct_current);
void hexdig(unsigned char **current_char, node *struct_current);
void crlf(unsigned char **current_char, node *struct_current);
void http_name(unsigned char **current_char, node *struct_current);
void http_version(unsigned char **current_char, node *struct_current);
void field_name(unsigned char **current_char, node *struct_current);
void ows(unsigned char **current_char, node *struct_current);
void htab(unsigned char **current_char, node *struct_current);
void field_content(unsigned char **current_char, node *struct_current);
void obs_fold(unsigned char **current_char, node *struct_current);
void field_vchar(unsigned char **current_char, node *struct_current);
void field_value(unsigned char **current_char, node *struct_current);
void vchar(unsigned char **current_char, node *struct_current);
void obs_text(unsigned char **current_char, node *struct_current);
void message_body(unsigned char **current_char, node *struct_current);
void octet(unsigned char **current_char, node *struct_current);
void header_field(unsigned char **current_char, node *struct_current);
void connection_header(unsigned char **current_char, node *struct_current);
void host_header(unsigned char **current_char, node *struct_current);
void content_length_header(unsigned char **current_char, node *struct_current);
void content_type_header(unsigned char **current_char, node *struct_current);
void cookie_string(unsigned char **current_char, node *struct_current);
void transfer_encoding_header(unsigned char **current_char, node *struct_current);
void expect_header(unsigned char **current_char, node *struct_current);
void connection_option(unsigned char **current_char, node *struct_current);
void uri_host(unsigned char **current_char, node *struct_current);
void host(unsigned char **current_char, node *struct_current);
void port(unsigned char **current_char, node *struct_current);
void ipv4address(unsigned char **current_char, node *struct_current);
void ipvfuture(unsigned char **current_char, node *struct_current);
void ip_literal(unsigned char **current_char, node *struct_current);
void reg_name(unsigned char **current_char, node *struct_current);
void dec_octet(unsigned char **current_char, node *struct_current);
void port(unsigned char** current_char, node* struct_current);
void ipv6address(unsigned char **current_char, node *struct_current);
void reg_name(unsigned char** current_char, node* struct_current);
void media_type(unsigned char **current_char, node *struct_current);
void type(unsigned char **current_char, node *struct_current);
void subtype(unsigned char **current_char, node *struct_current);
void parameter(unsigned char **current_char, node *struct_current);
void quoted_string(unsigned char **current_char, node *struct_current);
void qdtext(unsigned char **current_char, node *struct_current);
void quoted_pair(unsigned char **current_char, node *struct_current);
void h16(unsigned char **current_char, node *struct_current);
void ls32(unsigned char **current_char, node *struct_current);
void cookie_pair(unsigned char **current_char, node *struct_current);
void cookie_name(unsigned char **current_char, node *struct_current);
void cookie_value(unsigned char **current_char, node *struct_current);
void cookie_octet(unsigned char **current_char, node *struct_current);
void istring(unsigned char **current_char, node *struct_current, int length);
void transfer_coding(unsigned char **current_char, node *struct_current);
void transfer_parameter(unsigned char **current_char, node *struct_current);
void transfer_extension(unsigned char **current_char, node *struct_current);
