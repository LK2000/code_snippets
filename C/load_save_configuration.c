/**
 * populate thinclient_info struct from a persistent file
 *
 * @return 0 on success, -1 on failure
 ******************************************************************************/
int
load_configuration()
{
    FILE *fp;
    char *cptr;
    char  buf[1024];
    
    memset(&g_tci, 0, sizeof(struct thinclient_info));

    strcpy(g_tci.model, "undefined");
    strcpy(g_tci.description, "undefined");
    strcpy(g_tci.kernel_version, "undefined");
    strcpy(g_tci.rfs_version, "undefined");
    strcpy(g_tci.resolution, "undefined");
        
    /* open config file */
    if ((fp = fopen(CONFIG_FILE_NAME, "r")) == NULL)
    {
        log_error("error opening configuration file: %s; using defaults\n", CONFIG_FILE_NAME);
        return -1;
    }
    
    /* read and process entries */
    while (fgets(buf, 1024, fp) != NULL)
    {
        if (strcasestr(buf, "model="))
        {
            strcpy(g_tci.model, &buf[strlen("model=")]);
            if ((cptr = strstr(g_tci.model, "\n")) != NULL)
                *cptr = 0;
        }

        else if (strcasestr(buf, "description="))
        {
            strcpy(g_tci.description, &buf[strlen("description=")]);
            if ((cptr = strstr(g_tci.description, "\n")) != NULL)
                *cptr = 0;            
        }

        else if (strcasestr(buf, "kernel_version="))
        {
            strcpy(g_tci.kernel_version, &buf[strlen("kernel_version=")]);
            if ((cptr = strstr(g_tci.kernel_version, "\n")) != NULL)
                *cptr = 0;             
        }

        else if (strcasestr(buf, "rfs_version="))
        {
            strcpy(g_tci.rfs_version, &buf[strlen("rfs_version=")]);
            if ((cptr = strstr(g_tci.rfs_version, "\n")) != NULL)
                *cptr = 0; 
        }
            
        else if (strcasestr(buf, "resolution="))
        {
            strcpy(g_tci.resolution, &buf[strlen("resolution=")]);
                if ((cptr = strstr(g_tci.resolution, "\n")) != NULL)
                *cptr = 0; 
        }
    }
    
    fclose(fp);
    
    log_debug("model=%s description=%s kernel_version=%s "
              "rfs_version=%s resolution=%s\n",
              g_tci.model, g_tci.description, g_tci.kernel_version,
              g_tci.rfs_version, g_tci.resolution);
    
    return 0;
}

/**
 * save data from thinclient_info struct to a persistent file
 *
 * @return 0 on success, -1 on failure
 ******************************************************************************/
int
save_configuration()
{
    FILE *fp;
    
    log_debug("model=%s description=%s kernel_version=%s "
              "rfs_version=%s resolution=%s\n",
              g_tci.model, g_tci.description, g_tci.kernel_version,
              g_tci.rfs_version, g_tci.resolution);
    
    if ((fp = fopen(CONFIG_FILE_NAME, "w")) == NULL)
    {
        log_error("error opening configuration file: %s\n", CONFIG_FILE_NAME);
        return -1;
    }
    
    fprintf(fp, "model=%s\n", g_tci.model);
    fprintf(fp, "description=%s\n", g_tci.description);
    fprintf(fp, "kernel_version=%s\n", g_tci.kernel_version);
    fprintf(fp, "rfs_version=%s\n", g_tci.rfs_version);
    fprintf(fp, "resolution=%s\n", g_tci.resolution);
    
    fclose(fp);
    
    return 0;
}
