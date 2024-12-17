#include "myWifi.h"
#include "lwip/apps/httpd.h"
#include "lwipopts.h"
#include "pico/cyw43_arch.h"
#include "lwip/tcp.h"
#include "lwip/netif.h"
#include "Drivers/cJSON.h"
#include "Drivers/commandExec.h"

char* connect_to_wifi() {
    printf("Initializing Wi-Fi...\n");
    if (cyw43_arch_init()) {
        printf("Wi-Fi initialization failed.\n");
        return "Wi-Fi initialization failed";
    }

    // Connect to Wi-Fi
    cyw43_arch_enable_sta_mode();
    printf("Connecting to Wi-Fi...\n");
    while(cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0){
        printf("Attempting to connect...\n");
    }
    printf("Connected to Wi-Fi.\n");

    // Print the Pico's IP address
    const ip4_addr_t *ip = netif_ip4_addr(netif_list);
    printf("Pico W IP address: %s\n", ip ? ip4addr_ntoa(ip) : "No IP");
    return ip ? ip4addr_ntoa(ip) : "No IP";
}

const char *extract_json(const char *data);
void json_parser(const char *json_payload);
static err_t http_server_accept(void *arg, struct tcp_pcb *new_pcb, err_t err);
static err_t http_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);

void http_server_init(){
    struct tcp_pcb *pcb;

    // Create a new TCP protocol control block
    pcb = tcp_new();
    if (pcb == NULL) {
        printf("Error creating PCB\n");
        return;
    }

    // Bind to port 80 (HTTP)
    err_t err = tcp_bind(pcb, IP_ADDR_ANY, 80);
    if (err != ERR_OK) {
        printf("Error binding PCB: %d\n", err);
        return;
    }

    // Start listening for incoming connections
    pcb = tcp_listen(pcb);
    if (pcb == NULL) {
        printf("Error setting PCB to listen\n");
        return;
    }

    // Set the accept callback
    tcp_accept(pcb, http_server_accept);

    printf("HTTP server initialized and listening on port 80\n");

}



// Callback function to handle incoming requests
static err_t http_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p == NULL) {
        // Client closed the connection
        tcp_close(tpcb);
        printf("Client closed the connection\n");
        return ERR_OK;
    }

    // Log the received data
    printf("Received data: %.*s\n", p->len, (char *)p->payload);

    printf("Received data \n");


    char *payload = (char *)malloc(p->len + 1);
    if (!payload) {
        printf("Memory allocation failed\n");
        return ERR_MEM;
    }
    memcpy(payload, p->payload, p->len);
    payload[p->len] = '\0';

    // Extract the JSON portion from the payload
    const char *json_payload = extract_json(payload);
    if (json_payload) {
        // printf("Extracted JSON: %s\n", json_payload);
        json_parser(json_payload);
    } else {
        printf("No JSON found in payload.\n");
    }

    free(payload);


    // Define the HTTP response
    const char *simple_response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 2\r\n"
        "Connection: close\r\n"
        "\r\n"
        "OK";

    // Acknowledge received data
    tcp_recved(tpcb, p->tot_len);

    // Write the response
    err_t write_err = tcp_write(tpcb, simple_response, strlen(simple_response), TCP_WRITE_FLAG_COPY);
    // printf("Response written: %d\n", write_err);

    if (write_err == ERR_OK) {
        err_t output_err = tcp_output(tpcb);
        printf("Response flushed: %d\n", output_err);
    } else {
        printf("Error in tcp_write: %d\n", write_err);
    }

    // Free the received buffer
    pbuf_free(p);

    // Delay to ensure the response is transmitted
    sleep_ms(10);

    // Close the connection
    err_t close_err = tcp_close(tpcb);
    // printf("Connection closed: %d\n", close_err);

    return ERR_OK;
}

// // Callback function to accept new connections
static err_t http_server_accept(void *arg, struct tcp_pcb *new_pcb, err_t err) {
    printf("New connection accepted\n");
    tcp_recv(new_pcb, http_server_recv);
    return ERR_OK;
}

// Function to extract JSON from the HTTP payload
const char *extract_json(const char *data) {
    // Find the double newline separating headers and body
    const char *json_start = strstr(data, "\r\n\r\n");
    if (json_start) {
        json_start += 4; // Skip the double newline
        return json_start;
    }
    return NULL; // No JSON found
}

// Function to parse and print JSON attributes
void json_parser(const char *json_payload) {
    // Parse the JSON payload
    cJSON *json = cJSON_Parse(json_payload);
    if (json == NULL) {
        printf("Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }

        // Get the "type" field
    cJSON *type_item = cJSON_GetObjectItem(json, "type");
    if (!cJSON_IsString(type_item) || (type_item->valuestring == NULL)) {
        printf("Invalid Request\n");
        cJSON_Delete(json);
        return;
    }

    // pass the json to the command executer
    json_executer(json);

    // Free the JSON object
    cJSON_Delete(json);
}