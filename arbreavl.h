/* UQAM / Département d'informatique
   INF3105 - Structures de données et algorithmes
   Squelette pour classe générique ArbreAVL<T> pour le Lab6 et le TP2.

   AUTEUR(S):
    1) Nom + Code permanent du l'étudiant.e 1
    2) Nom + Code permanent du l'étudiant.e 2
*/

#if !defined(__ARBREAVL_H__)
#define __ARBREAVL_H__
#include <cassert>
#include "pile.h"
#include <iostream>

template <class T> class ArbreAVL {
  public:
    ArbreAVL();
    ArbreAVL(const ArbreAVL&);
    ~ArbreAVL();

    // Annonce l'existance d'une classe Iterateur.
    class Iterateur;

    void inserer(const T&);
    bool contient(const T&) const;
    bool vide() const;
    void vider();
    void enlever(const T&); // non requis pour le TP2.
    int  hauteur() const;
    
    // Fonctions pour obtenir un itérateur (position dans l'arbre)    
    Iterateur debut() const;
    Iterateur fin() const;
    Iterateur rechercher(const T&) const;
    Iterateur rechercherEgalOuSuivant(const T&) const;
    Iterateur rechercherEgalOuPrecedent(const T&) const;

    // Accès aux éléments de l'arbre via un intérateur.
    const T& operator[](const Iterateur&) const;
    T& operator[](const Iterateur&);
    
    // Copie d'un arbre AVL.
    ArbreAVL& operator = (const ArbreAVL&);

  private:
    struct Noeud{
        Noeud(const T&);
        T contenu;
        int equilibre;
        Noeud *gauche,*droite;
    };

    Noeud* racine;

    // Fonctions internes
    bool inserer(Noeud*&, const T&);
    void rotationGaucheDroite(Noeud*&);
    void rotationDroiteGauche(Noeud*&);
    void vider(Noeud*&);
    void copier(const Noeud*, Noeud*&) const;
    const T& max(Noeud*) const;
    bool enlever(Noeud*&, const T& e);

  public:
    // Sera présenté à la semaine #7
    class Iterateur{
      public:
        Iterateur(const ArbreAVL& a);
        Iterateur(const Iterateur& a);
        Iterateur(const ArbreAVL& a, Noeud* c);

        operator bool() const;
        bool operator!() const;
        bool operator==(const Iterateur&) const;
        bool operator!=(const Iterateur&) const;
        
        const T& operator*() const;

        Iterateur& operator++();
        Iterateur operator++(int);
        Iterateur& operator = (const Iterateur&);
      private:
        const ArbreAVL& arbre_associe;
        Noeud* courant;
        Pile<Noeud*> chemin;
        
      friend class ArbreAVL;
    };
};


//--------------------------------Définition-------------------------------------


//Constructeur d'un noeud avec une valeur 
template <class T> ArbreAVL<T>::Noeud::Noeud(const T& c) 
: contenu(c), 
equilibre(0), 
gauche(NULL),
droite(NULL) {}

//constructeur de l'arbre sans paramètre
template <class T> ArbreAVL<T>::ArbreAVL() 
 : racine(NULL)
{
}

//constructeur de copie d'un arbre 
template <class T> ArbreAVL<T>::ArbreAVL(const ArbreAVL<T>& autre)
 : racine(NULL)
{
    copier(autre.racine, this->racine);
}

//destructeur
template <class T> ArbreAVL<T>::~ArbreAVL()
{
    vider(this->racine);
}

template <class T> bool ArbreAVL<T>::contient(const T& element) const
{
	bool contient = false;
	
	Noeud* tmp = this->racine;

	while(tmp != NULL && contient == false){

		if(element < tmp->contenu){

			tmp = tmp->gauche;

		} else if(element > tmp->contenu){

			tmp = tmp->droite;

		} else {

			contient = true;

		}

	}

	return contient;

}

//ajouter normal.
template <class T>
void ArbreAVL<T>::inserer(const T& element)
{
    inserer(racine, element);
}

template <class T> 
bool ArbreAVL<T>::inserer(Noeud*& noeud, const T& element)
{
	//mettre la feuille
    if(noeud==NULL) {

        noeud = new Noeud(element);
        return true;

    }

    if(element < noeud->contenu){

    	//insertion à gauche de ce noeud

        if(inserer(noeud->gauche, element)) {

            noeud->equilibre++;
            if(noeud->equilibre == 0) return false;
            if(noeud->equilibre == 1) return true;
            assert(noeud->equilibre == 2);
            if(noeud->gauche->equilibre == -1) rotationDroiteGauche(noeud->gauche);
            rotationGaucheDroite(noeud);

        }

        return false;

    }

    else if(noeud->contenu < element){

    	if(inserer(noeud->droite, element)){

			noeud->equilibre--;
			if(noeud->equilibre == 0) return false;
            if(noeud->equilibre == -1) return true;

            assert(noeud->equilibre == -2);
            if(noeud->droite->equilibre == 1) rotationGaucheDroite(noeud->droite);

            rotationDroiteGauche(noeud);

    	}

        return false;

    }

    // element == noeud->contenu
    noeud->contenu = element;  // Mise à jour
    return false;

}

template <class T> void ArbreAVL<T>::rotationGaucheDroite(Noeud*& racinesousarbre)
{
    Noeud *temp = racinesousarbre->gauche;
    int  ea = temp->equilibre;
    int  eb = racinesousarbre->equilibre;
    int  neb = -(ea > 0 ? ea : 0) - 1 + eb;
    int  nea = ea + (neb < 0 ? neb : 0) - 1;

    // faire la rotation
    temp->equilibre = nea;
    racinesousarbre->equilibre = neb;
    racinesousarbre->gauche = temp->droite;
    temp->droite = racinesousarbre;
    racinesousarbre = temp;
}

template <class T> void ArbreAVL<T>::rotationDroiteGauche(Noeud*& racinesousarbre)
{
    // À compléter.
    Noeud *temp = racinesousarbre->droite; //B
    int eb = temp->equilibre;
    int ea = racinesousarbre->equilibre;
    int nea = -(eb < 0 ? eb : 0) + 1 + ea;
    int neb = (nea > 0 ? nea : 0) + 1 + eb;

    temp->equilibre = neb;
    racinesousarbre->equilibre = nea;
    racinesousarbre->droite = temp->gauche;
    temp->gauche = racinesousarbre;
    racinesousarbre = temp;
}

template <class T> bool ArbreAVL<T>::vide() const
{
    return this->racine == NULL;
}

template <class T> void ArbreAVL<T>::vider(){
  vider(racine);
}

template <class T>
void ArbreAVL<T>::vider(Noeud*& noeud)
{
    
	if(noeud->gauche != NULL){

		vider(noeud->gauche);

	}

	if (noeud->droite != NULL){
		
		vider(noeud->droite); 
	}

	noeud = NULL;
	delete noeud;
}

template <class T>
void ArbreAVL<T>::copier(const Noeud* source, Noeud*& noeud) const
{
    noeud = new Noeud(source->contenu);
    noeud->equilibre = source->equilibre;
    copier(source->gauche, noeud->gauche);
    copier(source->droite, noeud->droite);

    
}

template <class T>
int  ArbreAVL<T>::hauteur() const{

    int hauteur = 0;

    Noeud* tmp = this->racine;

    while(tmp != NULL){

    	if (tmp->equilibre >= 0){

    		tmp = tmp->gauche;

    	} else {

    		tmp = tmp->droite;
    	}

    	hauteur++;
    }

    return hauteur;
}

template <class T>
const T& ArbreAVL<T>::max(Noeud* n) const
{
    while(n->droite != NULL){
    	n = n->droite;
    }

    return n->contenu;
}

// L'enlèvement est optionnel (non requise pour le TP2)
template <class T>
void ArbreAVL<T>::enlever(const T& element)
{
    enlever(racine, element);
}

template <class T>
bool ArbreAVL<T>::enlever(Noeud*& noeud, const T& element)
{
    if(element < noeud->contenu)
    {
        if(enlever(noeud->gauche, element))
        {
            // ...
        }
    }
    else if(element > noeud->contenu)
    {
        // ...
    }
    else if(element == noeud->contenu)
    {
        if (noeud->gauche==NULL && noeud->droite==NULL)
        {
            delete noeud;
            noeud = NULL;
            return true;
        }
        else
        {
            // ...
            return true;
        }
    }
    
}
//----------- L'enlèvement est optionnel (non requise pour le TP2) ----------//

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::debut() const
{
    Iterateur iter(*this);
    // À compléter.
    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::fin() const
{
    return Iterateur(*this);
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercher(const T& e) const
{
    Iterateur iter(*this);
    // À compléter.
    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuSuivant(const T& e) const
{
    Iterateur iter(*this);
    // À compléter.
    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuPrecedent(const T& e) const
{
    // À compléter.
    return Iterateur(*this);
}

template <class T>
const T& ArbreAVL<T>::operator[](const Iterateur& iterateur) const
{
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}

template <class T>
T& ArbreAVL<T>::operator[](const Iterateur& iterateur)
{
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}

template <class T>
ArbreAVL<T>& ArbreAVL<T>::operator=(const ArbreAVL& autre) {
    if(this==&autre) return *this;
    vider();
    copier(autre.racine, racine);
    return *this;
}

//-----------------------
template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL& a)
 : arbre_associe(a), courant(NULL)
{
}

template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL<T>::Iterateur& a)
: arbre_associe(a.arbre_associe)
{
    courant = a.courant;
    chemin = a.chemin;
}

// Pré-incrément
template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator++()
{
    // À compléter.
    return *this;
}

// Post-incrément
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::Iterateur::operator++(int)
{
    Iterateur copie(*this);
    operator++();
    return copie;
}

template <class T>
ArbreAVL<T>::Iterateur::operator bool() const
{
    return courant!=NULL;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!() const{
    return courant==NULL;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator==(const Iterateur& o) const{
    assert(&arbre_associe==&o.arbre_associe);
    return courant==o.courant;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!=(const Iterateur& o) const{
    assert(&arbre_associe==&o.arbre_associe);
    return courant!=o.courant;
}

template <class T>
const T& ArbreAVL<T>::Iterateur::operator *() const
{
    assert(courant!=NULL);
    return courant->contenu;
}

template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator = (const Iterateur& autre){
    assert(&arbre_associe==&autre.arbre_associe);
    courant = autre.courant;
    chemin = autre.chemin;
    return *this;
}

#endif
