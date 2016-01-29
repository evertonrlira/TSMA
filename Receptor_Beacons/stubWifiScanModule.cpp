#include "stubWifiScanModule.h"

StubWifiScanModule::StubWifiScanModule()
{
}

void StubWifiScanModule::Run() {
    while (1) {
        emit trafficSignReceived("UNK", "Sinal Desconhecido");
        sleep(3);

        emit trafficSignReceived("R1", "Parada Obrigatória");
        sleep(3);

        emit trafficSignReceived("R2", "Dê a Preferência");
        sleep(3);

        emit trafficSignReceived("R3", "Sentido Proibido");
        sleep(3);

        emit trafficSignReceived("R4A", "Proibido virar à Esquerda");
        sleep(3);

        emit trafficSignReceived("R4B", "Proibido virar à Direita");
        sleep(3);

        emit trafficSignReceived("R5A", "Proibido retornar à Esquerda");
        sleep(3);

        emit trafficSignReceived("R5B", "Proibido retornar à Direita");
        sleep(3);

        emit trafficSignReceived("R6A", "Proibido Estacionar");
        sleep(3);

        emit trafficSignReceived("R6B", "Estacionamento Regulamentado");
        sleep(3);

        emit trafficSignReceived("R6C", "Proibido Parar e Estacionar");
        sleep(3);
    }
}
