int send_version_code(char* code, char* version, int clientID);
<<<<<<< Updated upstream
int send_type_length(char* filename, int clientID, char* type);
int send_body(FILE* file, int clientID, int size);
=======
int send_type_length(char* filename, int clientID, char* final_mime_type);
int send_body(char* filename, int clientID, int size);
>>>>>>> Stashed changes
