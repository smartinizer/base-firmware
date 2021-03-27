#include "HttpsOTAUpdate.h"
// This sketch shows how to implement HTTPS firmware update Over The Air.
// Please provide your WiFi credentials, https URL to the firmware image and the server certificate.


static const char *url = "https://smartinizer.devzero.cloud/firmware.bin"; //state url of your firmware image

static const char *server_certificate = "-----BEGIN CERTIFICATE-----\n" \
    "MIIIGTCCBwGgAwIBAgISBGys8zuzIL+LjuWMmJwBLEDdMA0GCSqGSIb3DQEBCwUA\n" \
    "MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD\n" \
    "EwJSMzAeFw0yMTAzMDMxNTU1NDBaFw0yMTA2MDExNTU1NDBaMBoxGDAWBgNVBAMT\n" \
    "D21heGxhbXByZWNodC5kZTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIB\n" \
    "AMRd7bFta21omhTwkkW2uni7K9/QRAZDN1M/zFOwAZNJB5Pzz7bnc3z/JkoHUK7m\n" \
    "Cmfvp0tgsZIYOJcwY1I7VVEjgws8XTrNKuqI62ZYToQFRC5IfCwPXTvsUIfvVJUe\n" \
    "0n4jG8Aj9QN6EfIjN8srGG+e/ffLYnNufSg2iKBWmAr8V7RBedLwDKKcsfDYKUTm\n" \
    "G4b5tBteY8s5BKt6ICDGBD0Lr6W09w3TrmoY+zqiKxv1JCTJ6y6/9RMRAR0gMZU6\n" \
    "Ggk26SDJbUhn22QisQFbHFdbDtbJoE+4kQXqtJUo+9KfeFHs2oaSee6v3ZwwJi1O\n" \
    "rLx+FlAbBUxom2bPHluknYGi1H4M8mhdusHF6WQQt4MWR6M2zHa6ZQzdH7m6QKHS\n" \
    "GyMhjql3697bn22kXcNPws+0NaU8usasrqnwSjZA4wz2F5IyVykdsMaqueKvJ3gG\n" \
    "rhSo2NDgm4sn2uoy8aXjA1bM8FWD0wysDDRvLDKu1yqG6kRM7JbDaTzFHfRq/XtI\n" \
    "yMt7uCTloetk7uaB883iaIPE4M226ybDFafsmXtHVRyu20ajmPkCoIEL2m3M6WkQ\n" \
    "2YTZ6YZmTk4xMER+j2l8SSTvnkWnJx4kgh1A5EDpBqGjo02kzopQI9X0ga7S/cSl\n" \
    "JePU5Bc/TAn0m0/+8VYaqznTPWp7RJIgKm/6CZLDo1i9AgMBAAGjggQ/MIIEOzAO\n" \
    "BgNVHQ8BAf8EBAMCBaAwHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMAwG\n" \
    "A1UdEwEB/wQCMAAwHQYDVR0OBBYEFDAc9hEnL1JH46T3+tHGdWfDuWaAMB8GA1Ud\n" \
    "IwQYMBaAFBQusxe3WFbLrlAJQOYfr52LFMLGMFUGCCsGAQUFBwEBBEkwRzAhBggr\n" \
    "BgEFBQcwAYYVaHR0cDovL3IzLm8ubGVuY3Iub3JnMCIGCCsGAQUFBzAChhZodHRw\n" \
    "Oi8vcjMuaS5sZW5jci5vcmcvMIICDAYDVR0RBIICAzCCAf+CEmFwaS5zaGlzaGFs\n" \
    "YW1wZS5kZYITYXV0aC5zaGlzaGFsYW1wZS5kZYIcYXV0b2Rpc2NvdmVyLm1heGxh\n" \
    "bXByZWNodC5kZYIQY2xvdWQucm9ic3BvdC5kZYISZGllLWZyaXNjaGUtYm94LmRl\n" \
    "gg1mYWxjLmF1ZnJhLmRlghZnaXRsYWIubWF4bGFtcHJlY2h0LmRlghRtYWlsLm1h\n" \
    "eGxhbXByZWNodC5kZYIPbWF4bGFtcHJlY2h0LmRlghltdXNpa2JvdDEubWF4bGFt\n" \
    "cHJlY2h0LmRlgg9uaHBsYW1wcmVjaHQuZGWCFm9mZmljZS5tYXhsYW1wcmVjaHQu\n" \
    "ZGWCE3Bhc3Nib2x0LnJvYnNwb3QuZGWCFnByb2plY3RzLmZhbGMuYXVmcmEuZGWC\n" \
    "H3JlZ2lzdHJ5LmdpdGxhYi5tYXhsYW1wcmVjaHQuZGWCF3NoYXJlLnZpcmRhLWJl\n" \
    "cmF0dW5nLmRlgg5zaGlzaGFsYW1wZS5kZYIUc3RhdHMuc2hpc2hhbGFtcGUuZGWC\n" \
    "F3N0YXR1cy5oYXBweWhhbXN0ZXIub3JnghZ3d3cuZGllLWZyaXNjaGUtYm94LmRl\n" \
    "ghN3d3cubWF4bGFtcHJlY2h0LmRlghN3d3cubmhwbGFtcHJlY2h0LmRlghZ6ZWl0\n" \
    "LnZpcmRhLWJlcmF0dW5nLmRlMEwGA1UdIARFMEMwCAYGZ4EMAQIBMDcGCysGAQQB\n" \
    "gt8TAQEBMCgwJgYIKwYBBQUHAgEWGmh0dHA6Ly9jcHMubGV0c2VuY3J5cHQub3Jn\n" \
    "MIIBBQYKKwYBBAHWeQIEAgSB9gSB8wDxAHcAXNxDkv7mq0VEsV6a1FbmEDf71fpH\n" \
    "3KFzlLJe5vbHDsoAAAF3+QQ+BAAABAMASDBGAiEA0CGuJvr1ZyzIu8S+g02qT48a\n" \
    "Mgd1gp+6uk8C3+Vw+P4CIQCLVt6mfRr8NAxvlCMLMMm5jd1iZVzppjmgCnC5lIFP\n" \
    "DQB2APZclC/RdzAiFFQYCDCUVo7jTRMZM7/fDC8gC8xO8WTjAAABd/kEP+IAAAQD\n" \
    "AEcwRQIgf7bFaSWUd0343FGGAfJbhW+jvQA5e9m+7c518Jx9+WYCIQDVOmiIVGHC\n" \
    "NjMiX7LpQoK6PNEADKp15StwKvEkI2+u5DANBgkqhkiG9w0BAQsFAAOCAQEAY4o3\n" \
    "OZvUy/BNQAp7i9Pb+P+YuC+0MKI2L1IeZEBygPzWHsq0kNMX4JgBanV4k6yerrq0\n" \
    "Cv81CSXEKHdn8m63qe30vbt5TUOEyYqy8+UfF3TMixvYkEeaGeySeVGXoKap7JR5\n" \
    "bUl8joNeG+mTdRTn6+yrl+nG53rPmpwKjKC3mzt+rhVTf5qVlAZ5QWzBcqJWP20q\n" \
    "616M47lShqQonZJ0xgm3XxzatrucQDFj+cHtDlMZGUrSNpyMalmPIkemYx3vOsFH\n" \
    "qfzC9SJNSuH77BHFrorQ+h+0Lunw38iFtjsln/Aj4x7rB+LSG0OROxB0x2Rj2rTE\n" \
    "WR032nyLYG7BXDe3cA==\n" \
    "-----END CERTIFICATE-----"; 

static HttpsOTAStatus_t otastatus;

void HttpEvent(HttpEvent_t *event)
{
    switch(event->event_id) {
        case HTTP_EVENT_ERROR:
            Serial.println("Http Event Error");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            Serial.println("Http Event On Connected");
            break;
        case HTTP_EVENT_HEADER_SENT:
            Serial.println("Http Event Header Sent");
            break;
        case HTTP_EVENT_ON_HEADER:
            Serial.printf("Http Event On Header, key=%s, value=%s\n", event->header_key, event->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            break;
        case HTTP_EVENT_ON_FINISH:
            Serial.println("Http Event On Finish");
            break;
        case HTTP_EVENT_DISCONNECTED:
            Serial.println("Http Event Disconnected");
            break;
    }
}

void startUpdate(){
    HttpsOTA.onHttpEvent(HttpEvent);
    Serial.println("Starting OTA");
    HttpsOTA.begin(url, server_certificate); 

    Serial.println("Please Wait it takes some time ...");
}

void ota_loop(){
    otastatus = HttpsOTA.status();
    if(otastatus == HTTPS_OTA_SUCCESS) { 
        Serial.println("Firmware written successfully. To reboot device, call API ESP.restart() or PUSH restart button on device");
    } else if(otastatus == HTTPS_OTA_FAIL) { 
        Serial.println("Firmware Upgrade Fail");
    }
    delay(1000);
}