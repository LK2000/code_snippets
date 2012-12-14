/**
 * read exact number of bytes from specified fd into buf
 * and return number of bytes read
 *
 * @param  fd              a valid file descriptor
 * @param  buf             place read data here
 * @param  bytes_to_read   number of bytes to read from fd
 * @param  bytes_read_ret  return actual number of bytes read from fd
 *
 * @return 0 on success, -1 on failure
 ******************************************************************************/
int
read_exact(int fd, char *buf, int bytes_to_read, int *bytes_read_ret)
{
    int rv = 0;
    int bytes_read = 0;
    int residual = bytes_to_read;
    
    *bytes_read_ret = 0;
    
    while (bytes_read < bytes_to_read)
    {
        rv = read(fd, &buf[bytes_read], residual);
        if (rv <= 0)
        {
            *bytes_read_ret = bytes_read;
            return -1;
        }
        bytes_read += rv;
        residual -= rv;
    }
    
    *bytes_read_ret = bytes_read;
    
    return 0;
}
