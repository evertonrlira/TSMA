#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <string>
#include <map>

using namespace std;

class Definitions
{
public:
    map<string, const char*>* TrafficSignDescriptions;
public:
    Definitions();
    const char* GetTrafficSignDescription(string signType);
};

Definitions::Definitions() {
    TrafficSignDescriptions = new map<string, const char*>();


    TrafficSignDescriptions->insert(std::make_pair("R00", "Sinalizacao Padrao"));
    TrafficSignDescriptions->insert(std::make_pair("R02", "De a Preferencia"));
    TrafficSignDescriptions->insert(std::make_pair("R03", "Sentido Proibido"));
    TrafficSignDescriptions->insert(std::make_pair("R01", "Parada Obrigatoria"));
    TrafficSignDescriptions->insert(std::make_pair("R05", "Proibido Retornar"));
    TrafficSignDescriptions->insert(std::make_pair("R07", "Proibido Ultrapassar"));
    TrafficSignDescriptions->insert(std::make_pair("R08", "Proibido Mudar de Faixa de Transito"));
    TrafficSignDescriptions->insert(std::make_pair("R09", "Proibido Transito de Veiculos de Carga"));
    TrafficSignDescriptions->insert(std::make_pair("R10", "Proibido Transito de Veiculo Automotores"));
    TrafficSignDescriptions->insert(std::make_pair("R11", "Proibido Transito de Veiculos de Tracao Animal"));
    TrafficSignDescriptions->insert(std::make_pair("R12", "Proibido Transito de Bicicletas"));
    TrafficSignDescriptions->insert(std::make_pair("R13", "Proibido Transito de Maquinas Agricolas"));
    TrafficSignDescriptions->insert(std::make_pair("R14", "Carga Maxima Permitida"));
    TrafficSignDescriptions->insert(std::make_pair("R15", "Altura Maxima Permitida"));
    TrafficSignDescriptions->insert(std::make_pair("R16", "Largura Maxima Permitida"));
    TrafficSignDescriptions->insert(std::make_pair("R17", "Peso Maximo Permitido por Eixo"));
    TrafficSignDescriptions->insert(std::make_pair("R18", "Comprimento Maximo Permitido"));
    TrafficSignDescriptions->insert(std::make_pair("R19", "Velocidade Maxima Permitida"));
    TrafficSignDescriptions->insert(std::make_pair("R20", "Proibido Acionar Buzina ou Sinal Sonoro"));
    TrafficSignDescriptions->insert(std::make_pair("R21", "Alfandega"));
    TrafficSignDescriptions->insert(std::make_pair("R22", "Uso Obrigatorio de Corrente"));
    TrafficSignDescriptions->insert(std::make_pair("R23", "Conserve-se a Direita"));
    TrafficSignDescriptions->insert(std::make_pair("R26", "Siga em Frente"));
    TrafficSignDescriptions->insert(std::make_pair("R27", "Veiculos Lentos usem Faixa da Direita"));
    TrafficSignDescriptions->insert(std::make_pair("R28", "Mao Dupla"));
    TrafficSignDescriptions->insert(std::make_pair("R29", "Proibido Transito de Pedestres"));
    TrafficSignDescriptions->insert(std::make_pair("R30", "Pedestre Ande pela Esquerda"));
    TrafficSignDescriptions->insert(std::make_pair("R31", "Pedestre Ande pela Direita"));
}

const char* Definitions::GetTrafficSignDescription(string signType) {
    if (TrafficSignDescriptions->find(signType) == TrafficSignDescriptions->end())
        return "Sinalizacao Desconhecida";
    return TrafficSignDescriptions->at(signType);
}

#endif // DEFINITIONS_H
