/* Checks if a directory exists
 * Returns -1 on err, 0 on failure, and 1 on success
*/
int directory_exists(const char * directoryToCheck);

/* Creates a directory using mkdir
 * Returns -1 on err, 0 on success, see man mkdir(2)
*/
int directory_create(const char * directoryToCheck);

/* Checks if a file exists or not. 
 * Returns 0 on false, 1 on success, see stat(2)
*/
int file_exists(const char * filename);

/* Checks if the file has been modified before lastCheckedTime
 * Returns 1 if so, 0 if the file has not been modified after the given time
*/
time_t file_last_modified_after(const char * filename, time_t lastCheckedTime);

/* Creates the data directory to store users and conversations
 * Returns 1 on success or if the file already exists and 0 on failure
*/
int create_data_dir();

/* Creates the file to contain the list of users who are allowed into the system
 * Returns 1 on sucess or if the file already exists
*/
int create_users_file();

/* Initializes data directory and files for PCI. Creating data dir and user dir
 * Returns 1 on sucess or 0 on failture
*/
int pci_init();