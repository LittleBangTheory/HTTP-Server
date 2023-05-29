int send_version_code(char* code, char* version, int clientID);
int send_type_length(char* filename, int clientID, char* type);
int send_body(FILE* file, int clientID, int size);