
/*
 * extract a 32bit value from specified buffer using little endian
 *
 * @param  value  place extracted 32bit value here
 * @param  buf    buffer containing raw data
 ******************************************************************************/
void
extract_32_le(uint32_t *value, char *buf)
{
    *value =  ((buf[3] << 24) & 0xff000000)
            | ((buf[2] << 16) & 0x00ff0000)
            | ((buf[1] <<  8) & 0x0000ff00)
            |  (buf[0]        & 0x000000ff);
}

/*
 * insert a 32bit value into specified buffer using little endian
 *
 * @param  buf    place 32bit value here
 * @param  value  value to place into buffer
 ******************************************************************************/
void
insert_32_le(char *buf, uint32_t value)
{
    buf[0] = value & 0xff;
    buf[1] = (value >>  8) & 0xff;
    buf[2] = (value >> 16) & 0xff;
    buf[3] = (value >> 24) & 0xff;
}
