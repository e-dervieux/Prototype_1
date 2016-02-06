#ifndef ERREUR_H
#define ERREUR_H

#include <string>
#include <exception>

class Erreur : public std::exception
{
    public:
        Erreur(int niveau, std::string description);
        ~Erreur() throw() {}

        const char* what() const throw();
        int getNiveau() {return m_niveau;}

        static std::string traduireNiveau(int niveau);

    protected:
    private:
        int m_niveau;
        std::string m_description;

        static std::string niveaux[];
};

#endif // ERREUR_H
