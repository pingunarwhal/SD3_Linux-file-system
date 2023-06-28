311CA Marin Iulia
313CA Constantinescu Delia
Tema 3 SD

Implementare generala:
 - am folosit implementarea de liste simplu inlantuite, cu
 cateva modificari

 - am construit 2 functii separate pentru adaugarea unui fisier
 sau a unui director in interiorul directorului curent

 - am construit 2 functii separate pentru stergerea unui nod din
 lista unui unui director, una care primeste ca parametru pozitia
 si una care primeste ca parametru numele resursei

find_node():
 - functie recursiva

 - functia cauta o resursa in ierarhia de directoare care
 porneste din nodul curent

 - in cazul in care gaseste resursa, o returneaza, in caz contrar,
 returneaza NULL

recursive_free():
 - functie recursiva

 - sterge toata ierarhia de directoare care porneste din nodul
 curent

 - in cazul in care nodul curent este un director ce contine alte
 resurse, functia se apeleaza recursiv

ls_dir():
 - functie ajutatoare care afiseaza intreg continutul unui
 director

cd():
 - foloseste functia find_node() pentru a accesa directorului

cdtree():
 - functie ajutatoare pentru tree()

 - la fel ca cd(), doar ca nu afiseaza mesajele de eroare

rmrec():
 - foloseste functia recursive_free() pentru a sterge resursele
 si pentru a elibera memoria

 - in cazul in care nu primeste argumente, functia sterge intreaga
 ierarhie incepand din directorulo curent

cp():
 - foloseste functia cdtree() pentru a obtine sursa

 - in cazul in care destinatia este o cale care nu exista
 dar poate fi creata, aceasta va fi creata

 - daca destinatia este un fisier care exista deja, doar 
 continutul acestuia va fi modificat