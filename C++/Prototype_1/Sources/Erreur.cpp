#include "../Header/Erreur.h"

std::string Erreur::niveaux[] = {"Mineure", "Warning", "Fatale", "Catastrophique", "Jambonique"};

Erreur::Erreur(int niveau, std::string description)
 : m_niveau(niveau), m_description(description)
{}

const char* Erreur::what() const throw()
{
    std::string s = "Erreur " + traduireNiveau(m_niveau) + " - " + m_description;
    return s.c_str();
}

std::string Erreur::traduireNiveau(int niveau)
{
    if (niveau < 0)
        return "Erreur sur le niveau de l'erreur........ GG";
    if (niveau >= 5)
        return "Au-dela de " + niveaux[4];
    else
        return niveaux[niveau];
}
