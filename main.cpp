#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <fstream>

std::ifstream f("grupa151.in");
std::ofstream g("grupa151.out");

/************************* Materie ******************************/
class Materie{
private:
    int credite;
    double medie;
    std::string nume;
    std::vector<double> note;
public:
	Materie(std::string);
    Materie(std::string, int);
    Materie(std::string, std::vector<double>);
    ~Materie();
    void set_credite(int);
    void set_nume(int);
    int get_credite();
    double get_medie();
    void add_nota(double);
    double calc_medie();
    bool operator==(const Materie& other);
};
Materie::Materie(std::string nume): nume(nume){}

Materie::Materie(std::string nume, int credite): nume(nume), credite(credite){}

Materie::Materie(std::string _nume, std::vector<double> _note){
    nume = nume;
    note = _note;
    medie = calc_medie();
}

Materie::~Materie(){
    note.clear();
    nume.clear();
}

void Materie::set_credite(int _credite){
    credite = _credite;
}

void Materie::set_nume(int _nume){
    nume = _nume;
}

int Materie::get_credite(){
    return credite;
}

double Materie::get_medie(){
    return medie;
}

void Materie::add_nota(double x){
    note.push_back(x);
    medie = calc_medie();
}

double Materie::calc_medie(){
    double suma = 0.0;
    for(auto nota: note)
        suma += nota;
    return suma / (double)note.size();
}

bool Materie::operator==(const Materie& other){
    return this->nume == other.nume;
}

/************************** Student ******************************/

class Student {
private:
    std::string nume;
    int an_nastere;
    int numar_credite;
    double medie_generala; //membrii privati
    std::vector<Materie> materii;
public:
    Student(std::string nume, int an_nastere); // constructor de initializare
    Student(std::string, int, std::vector<Materie>);
    Student(const Student&); //constructor de copiere
    ~Student(); //destructor
    std::string getNume();
    void setNume(std::string);
    double get_numar_credite();
    double get_medie_generala();
    void add_materie(Materie&);
    void add_nota_materie(Materie&, double);
    double calc_numar_credite();
    double calc_medie_generala();
    bool operator==(Student&);
    bool operator<(Student&);
    friend std::ostream& operator << (std::ostream& stream, Student& student); //supraincarcarea operatorului <<
    // friend std::istream& operator >> (std::istream& stream, Student& student); //supraincarcarea operatorului >>
};

Student::Student(std::string nume, int an_nastere) {
   this->nume=nume;
   this->an_nastere=an_nastere;
   this->numar_credite=0.0;
   this->medie_generala=0.0;
} //functia pentru constructor

Student::Student(std::string _nume, int _an_nastere, std::vector<Materie> _materii){
    nume = _nume;
    an_nastere = _an_nastere;
    materii = _materii;
    numar_credite = calc_numar_credite();
    medie_generala = calc_medie_generala();
}

Student::Student(const Student& student) {
    nume= student.nume;
    an_nastere=student.an_nastere;
    numar_credite=student.numar_credite;
    medie_generala=student.medie_generala;
    materii = student.materii;
} //functia constr de copiere

Student::~Student() {
    materii.clear();
    nume.clear();
} 

std::string Student::getNume() {
    return this->nume;
}

void Student::setNume(std::string nume) {
    this->nume = nume;
}

double Student::get_numar_credite(){
    return numar_credite;
}

double Student::get_medie_generala(){
    return medie_generala;
}

void Student::add_materie(Materie& materie){
    materii.push_back(materie);
}

void Student::add_nota_materie(Materie& materie, double nota){
    int gasit = 0;
    for(auto& mat: materii)
        if(mat == materie){
            mat.add_nota(nota);
            gasit = 1;
        }
    if(!gasit){
        materie.add_nota(nota);
        materii.push_back(materie);
    }
    numar_credite = calc_numar_credite();
    medie_generala = calc_medie_generala();
}

double Student::calc_numar_credite(){
    double credite = 0;
    for(auto& materie: materii)
        credite += materie.get_credite() * materie.get_medie() / 10.0;
    return credite;
}

double Student::calc_medie_generala(){
    double total_credite = 0;
    for(auto& materie: materii)
        total_credite += materie.get_credite();
    return numar_credite / total_credite * 10.0;
}

bool Student::operator<(Student& other){
    return this->medie_generala >= other.medie_generala;
}

bool Student::operator==(Student& other){
    return this->nume == other.nume && this->an_nastere == other.an_nastere;
}

std::ostream& operator<<(std::ostream& stream, Student& student) {
    stream << "Studentul " << student.getNume() << " are media generala " << student.get_medie_generala() << " si " << student.get_numar_credite() << " credite\n";
    return stream;
}


/************************** Grupa ******************************/

class Grupa {
private:
    //vector la "student" declarat dinamic
    int nr_studenti;
    int nr_grupa;
    double medie_grupa;
    std::vector<Student> studenti;
public:
	Grupa();
	Grupa(int);
    Grupa(int, std::vector<Student> studenti); //constr
    ~Grupa(); //destr
    int get_numar_grupa();
    double get_medie_grupa();
    void eliminaStudent(Student st); //functie elimina student
    void adaugaStudent(Student st); //functie adauga student 
    double calc_medie_grupa();
    void afisare_studenti();
	void adauga_nota_student(std::string, int, double, Materie&);
	std::vector<Student> get_studenti();
    Grupa operator+(Grupa&);
    friend std::ostream& operator << (std::ostream& stream, Grupa& grupa);
};
Grupa::Grupa(): nr_studenti(0) {}

Grupa::Grupa(int nr_grupa): nr_grupa(nr_grupa), nr_studenti(0), medie_grupa(0){}

Grupa::Grupa(int _nr_grupa, std::vector<Student> _studenti){
    this->nr_grupa = _nr_grupa;
    this->studenti=_studenti;
    this->nr_studenti=this->studenti.size();
    this->medie_grupa = this->calc_medie_grupa();
}

Grupa::~Grupa() {
    studenti.clear();
}

int Grupa::get_numar_grupa(){
    return nr_grupa;
}

double Grupa::get_medie_grupa(){
    return medie_grupa;
}

std::vector<Student> Grupa::get_studenti(){
	return studenti;
}

void Grupa::eliminaStudent(Student st){
    for(int i = 0; i < studenti.size(); i ++)
        if(studenti[i] == st){
            studenti.erase(studenti.begin() + i);
            nr_studenti --;
            medie_grupa = calc_medie_grupa();
            return;
        }
}

void Grupa::adaugaStudent(Student st){
    studenti.push_back(st);
    nr_studenti ++;
    medie_grupa = calc_medie_grupa();
}  

double Grupa::calc_medie_grupa(){
    double suma = 0.0;
    for(auto& st: studenti){
        suma += st.get_medie_generala();
    }
    return suma / (double)nr_studenti;
}

void Grupa::afisare_studenti(){
    std::sort(studenti.begin(), studenti.end());
    for(auto& student: studenti)
        std::cout << student;
}

void Grupa::adauga_nota_student(std::string nume_student, int an_nastere, double nota, Materie& materie){
	Student st{nume_student, an_nastere};
	for(int i = 0; i < nr_studenti; ++ i)
		if(studenti[i] == st){
			studenti[i].add_nota_materie(materie, nota);
			break;
		}
}

Grupa Grupa::operator+(Grupa& other){
    Grupa res;
    res.nr_grupa = std::min(this->nr_grupa, other.nr_grupa);
    res.nr_studenti = this->nr_studenti + other.nr_studenti;
    for(auto st: this->studenti)
        res.adaugaStudent(st);
    for(auto st: other.studenti)
        res.adaugaStudent(st);
    return res;
}

std::ostream& operator << (std::ostream& stream, Grupa& grupa){
    stream << "Grupa " << grupa.nr_grupa << " are " << grupa.nr_studenti << " studenti iar media grupei este " << grupa.medie_grupa << "\n";
    return stream;
}

int main(){
	// std::vector<Materie> materii;
	// materii.push_back()

	// Grupa grupa151(151);
	// grupa151.adaugaStudent({"Birsan Vlad Ioan", 2002});
	// grupa151.adaugaStudent({"Popa Stefan", 2003});
	// grupa151.adaugaStudent({"Costescu Matei", 2002});

	int nr_materii, nr_studenti, nr_note, credite, an_nastere;
	double nota;
	char new_line;
	std::string nume_student, nume_materie;
	Grupa grupa151(151);
	
	f >> nr_studenti;
	new_line = f.get();
	for(int i = 0; i < nr_studenti; i ++){
		std::vector<Materie> materii;
		std::getline(f, nume_student);
		f >> an_nastere >> nr_materii;
		new_line = f.get();
		for(int j = 0; j < nr_materii; ++ j){
			std::getline(f, nume_materie);
			f >> credite >> nr_note;
			Materie materie{nume_materie, credite};
			for(int k = 0; k < nr_note; ++ k){
				f >> nota;
				materie.add_nota(nota);
			}
			new_line = f.get();
			materii.push_back(materie);
		}
		grupa151.adaugaStudent({nume_student, an_nastere, materii});
	}
	g << grupa151 << "\n";
	Materie mat{"Tehnici Web"};
	grupa151.adauga_nota_student("Buzas Radu Gabriel", 2002, 5, mat);
	std::vector<Student> studenti = grupa151.get_studenti();
	for(int i = 0; i < studenti.size(); i ++)
		g << studenti[i] << "\n";

    return 0;
}