int send_version_code(char* code, char* version, int clientID);
int send_type_length(char* filename, int clientID, char* final_mime_type);
int send_body(char* filename, int clientID, int size);
