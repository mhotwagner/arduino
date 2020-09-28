Logger();

  	bool apiConfigured();
    void setApi();
    
    bool useApi();
    void useSerial();

    void logSerial(String message);
    bool logApi(String message);
    void log(String message);

    bool usingApi = false;
    const char* apiHost;