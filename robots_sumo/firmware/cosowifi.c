#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/tcp.h"   // AGREGADO: API TCP de lwIP

#define TCP_PORT 4242   // AGREGADO: puerto donde escucha la Pico

// AGREGADO: se ejecuta cada vez que llegan datos por WiFi
static err_t recv_cb(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
    if (!p) {                       // el cliente cerró la conexión
        tcp_close(pcb);
        return ERR_OK;
    }

    for (struct pbuf *q = p; q != NULL; q = q->next) {
        const char *data = (const char *)q->payload;
        for (u16_t i = 0; i < q->len; i++) {
            char c = data[i];
            if (c == 'w' || c == 'W') {
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);   // prende el LED
                printf("Letra 'w' recibida -> LED encendido\n");
            } else if (c == 'x' || c == 'X') {
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);   // apaga el LED
                printf("Letra 'x' recibida -> LED apagado\n");
            }
        }
    }

    tcp_recved(pcb, p->tot_len);
    pbuf_free(p);
    return ERR_OK;
}

// AGREGADO: se ejecuta cuando alguien se conecta a la Pico
static err_t accept_cb(void *arg, struct tcp_pcb *client_pcb, err_t err)
{
    if (err != ERR_OK || client_pcb == NULL) {
        return ERR_VAL;
    }
    tcp_recv(client_pcb, recv_cb);
    printf("Cliente conectado\n");
    return ERR_OK;
}

// AGREGADO: inicia el servidor TCP
static bool tcp_server_start(void)
{
    cyw43_arch_lwip_begin();

    struct tcp_pcb *pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
    if (!pcb) {
        cyw43_arch_lwip_end();
        return false;
    }
    if (tcp_bind(pcb, NULL, TCP_PORT) != ERR_OK) {
        tcp_close(pcb);
        cyw43_arch_lwip_end();
        return false;
    }
    struct tcp_pcb *listen_pcb = tcp_listen_with_backlog(pcb, 1);
    if (!listen_pcb) {
        tcp_close(pcb);
        cyw43_arch_lwip_end();
        return false;
    }
    tcp_accept(listen_pcb, accept_cb);

    cyw43_arch_lwip_end();
    return true;
}

int main()
{
    stdio_init_all();

    // Initialise the Wi-Fi chip
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }

    // Enable wifi station
    cyw43_arch_enable_sta_mode();

    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms("Cooperadora Profesores", "Profes_IMPA_2022", CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        return 1;
    } else {
        printf("Connected.\n");
        // Read the ip address in a human readable way
        uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
        printf("IP address %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
    }

    // AGREGADO: arranca el servidor que espera la letra 'w'
    if (!tcp_server_start()) {
        printf("No se pudo iniciar el servidor TCP\n");
    } else {
        printf("Servidor escuchando en el puerto %d\n", TCP_PORT);
    }

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
