#include <iostream>
#include <cmath>
#include <vector>
#include "MyException.h"
using namespace std;
struct Node
{
    int data,degree;
    bool mark=false;
    bool searched=false;
    Node *parent=NULL,*child=NULL,*left=NULL,*right=NULL;

};
void fibHeapInsert(Node*&Min, int new_data, int&n)
{

    n++;
    Node *new_node=new Node;
    new_node->data=new_data;
    new_node->degree=0;
    new_node->mark=false;
    new_node->parent=NULL;
    new_node->child=NULL;
    if(Min==NULL) ///verificam daca H este gol
    {

        new_node->left=new_node;
        new_node->right=new_node;

        ///la sfarsit setam pointer-ul Min catre nod
        Min=new_node;
        return;
    }
    ///altfel - daca heap-ul contine si alti arbori
    ///ideea este urmatoare: important este sa schimbi valoarea lui min->left->right inainte de cea a lui min->left
    ///altfel, nodul new_node o sa fie legat la dreapta de el insusi
    new_node->left=Min->left;
    Min->left->right=new_node;
    Min->left=new_node;
    new_node->right=Min;

    if(new_node->data<Min->data)
        Min=new_node;

}
void FibonacciHeap(Node*&Min, int&n)
{
    n=0;
    Min=NULL;
}
Node* fibHeapBuild(vector<int> v)
{
    Node *Min;
    int n=0;
    FibonacciHeap(Min,n);
    for(auto &x: v)
        fibHeapInsert(Min,x,n);
    return Min;

}
int findMin(Node*Min)
{
    return Min->data;
}
void fibHeapDisplayChildren(Node*x)
{
    if(x->child!=NULL)
    {

        cout<<"( Copii nodului "<<x->data<<" sunt : ";
        Node *it=x->child;
        do
        {
            if(it!=x->child)cout<<", ";
            cout<<it->data;

            fibHeapDisplayChildren(it);
            it=it->right;

        }
        while(it!=x->child);
        cout<<" endoflist)\n";
    }


}
void fibHeapDisplay(Node *Min, int n=0)
{
    Node*start=Min;
    if(start==NULL)
    {
        cout<<"Heap-ul este gol\n";
        return;
    }
    cout<<"Heapul are "<<n<<" noduri si valorile: \n";
    do
    {
        if (start != Min)
        {
            cout << "-->";
        }
        cout<<start->data<<" ";
        fibHeapDisplayChildren(start);

        start=start->right;

    }
    while(start!=Min);

   cout<<"\n";
    }

void fibHeapLink(Node *&x, Node*&y)
{


    Node *new_child=y;
    ///il eliminam pe y din root list
    y->left->right=y->right;
    y->right->left=y->left;
    x->degree++;

    new_child->left=new_child;
    new_child->right=new_child;

    ///salvam copilul lui x ca sa ne fie mai usor sa implementam legaturile
    Node*brother=x->child;
    if(brother!=NULL)
    {
        ///inseram y in dreapta copilului lui x
        new_child->right=brother->right;
        brother->right->left=new_child;
        brother->right=new_child;
        new_child->left=brother;

        ///il punem pe x sa pointeze catre cel mai mic copil
        if(new_child->data<brother->data)
            x->child=new_child;

    }
    else
        x->child=new_child;

    ///il facem pe x parinte a lui new_child
    new_child->parent=x;
    new_child->mark=false;

}
void add(Node*&Min,Node*new_node)
{

    if(Min!=NULL)
    {

        Min->right->left=new_node;
        new_node->right=Min->right;
        Min->right=new_node;
        new_node->left=Min;
        return;
    }

    Min=new_node;
    Min->right=Min;
    Min->left=Min;
}

void consolidate(Node*&Min, int n)
{

    int D=log2(n);
    Node* A[D+1];
    for(int i=0; i<=D+1; i++)
        A[i]=NULL;

    ///pentru fiecare nod din root list parcurgem urmatorul algoritm
    Node *it=Min;
    Node *x=it;
    do
    {

        int d=x->degree;
        while(A[d]!=NULL)
        {

            ///daca mai exista un nod de acelasi degree->trebuie sa facem link intre cele doua;
            Node *y=A[d];
            ///interschimbam intre ei pointerii; astfel x
            ///va pointa mereu catre nodul care are valoare retinuta
            ///mai mica - cel pe care vrem sa il facem parinte
            ///al celuilalt nod de acelasi degree
            if(x->data>y->data)
            {

                Node *aux=x;
                x=y;
                y=aux;
            }
            if(y==Min)
                Min=x;

            fibHeapLink(x,y);
            A[d]=NULL;
            d=d+1;

        }
        A[d]=x;
        x=x->right;
    }
    while(x!=Min);

    ///golim heap-ul nostru, fara a sterge insa nodurile din memorie
    ///o sa reconstruim heap-ul pe baza informatiilor din vectorul A
    Min=NULL;
    for(int i=0; i<=D+1; i++)
    {

        if(A[i]!=NULL)
        {

            add(Min,A[i]);
            ///actualizam Minimul
            if(Min==NULL || A[i]->data<Min->data)
                Min=A[i];
        }
    }

}
void fibHeapUnion(Node *&Min1,Node*&Min2, Node *&Min, int n1, int n2, int &n)
{
    FibonacciHeap(Min,n); ///cream un nou heap H
    Min=Min1; ///acum heap-ul nostru contine componentele heapului H1
    ///concatenam lista nodurilor lui H2 cu cele ale lui H
    ///folosim 2 pointeri temporari
    Node *last,*last2;
    last=Min->left;
    last2=Min2->left;

    Min->left->right=Min2;
    Min2->left->right=Min;
    Min->left=Min2->left;
    last2=last->right;
    n=n1+n2;

    if(Min1==NULL || (Min2!=NULL && Min2->data<Min1->data))
        Min=Min2;
}
void cut(Node*&Min,Node*&x,Node*&y)
{
    y->degree--;
    if(y->child==x && x->right!=x)
        y->child=x->right;
    else if(y->child==x && x->right==x)
        y->child=NULL;
    if(y->child!=NULL) ///daca are altii copii ->trebuie eliminat nodul
        ///din child list
    {
        x->right->left=x->left;
        x->left->right=x->right;
    }
    ///il formatam ca si o lista cu un singur nod
    x->right=x;
    x->left=x;
    ///il adaugam in root list, in stanga minimului
    Min->left->right=x;
    x->left=Min->left;
    x->right=Min;
    Min->left=x;


    x->parent=NULL;
    x->mark=false;

}
void cascading_cut(Node*&Min,Node*&y)
{
    Node*z=y->parent;
    if(z!=NULL)
    {

        if(y->mark==false)
        {

            y->mark=true; ///marcam faptul ca nodul si-a pierdut primul copil

        }
        else
        {
            cut(Min,y,z);
            cascading_cut(Min,z);
        }
    }
}
void fibHeapDecreaseKey(Node*&Min, Node*&x, int k)
{
    if(k>x->data)
    {
        throw(MyException("Valoare e mai mare ca valoarea nodului.",3));
    }
    x->data=k;
    Node*y=x->parent;
    ///verificam daca a fost incalcata regula pentru un heap min-ordered
    if(y!=NULL && x->data<y->data)
    {

        cut(Min,x,y);
        cascading_cut(Min,y);

    }
    ///actualizam minimul daca este cazul
    if(x->data<Min->data)
        Min=x;

}

Node* fibHeapExtractMin(Node*&Min, int &n)
{

    Node *kid=NULL,*stunt,*brother=NULL;
    stunt=Min;
    if(Min!=NULL)
    {

        ///Vom avea nevoie de trei pointeri de tip Node
        /// 1.) cel care sa prelucreze copii node - denumit kid
        /// 2.) unul care sa retina adresa fratelui nodului, inainte sa modificam legaturile nodului (si care face posibila iteratia prin child list) - denumit brother
        /// 3.) unul care sa retina adresa nodului Min, cel care ne va ajuta sa eliminam nodul minim din root list - denumit stunt
        if(stunt->child!=NULL)
        {
            ///adaugam fiecare copil a lui stunt in root list
            Node *kid=stunt->child;
            do
            {
                brother=kid->right;
                stunt->left->right=kid;
                kid->left=stunt->left;
                stunt->left=kid;
                kid->right=stunt;
                kid->parent=NULL;
                kid=brother;

            }
            while(brother!=stunt->child);
            ///scoatem stunt-ul din root list
        }

        stunt->left->right=stunt->right;
        stunt->right->left=stunt->left;

        ///daca H a fost creat dintr-un singur nod
        if(stunt==stunt->right)
        {
            Min=NULL;
        }
        else
        {
            ///altfel
            Min=stunt->right;
            stunt->right=stunt;
            stunt->left=stunt;

            consolidate(Min,n);
        }
        n--;

    }
    return stunt;
}
void fibHeapChild(Node*&Min,Node*&x, int new_node,int &n)
{

    n++;
    Node *kid=new Node;
    kid->data=new_node;
    kid->left=kid;
    kid->right=kid;
    kid->degree=0;
    kid->parent=x;
    x->degree++;
    if(x->child==NULL)
        x->child=kid;
    else
    {
        ///trebuie inserat in child list
        Node*brother=x->child;
        brother->left->right=kid;
        kid->left=brother->left;
        kid->right=brother;
        brother->left=kid;

    }

}
void fibHeapDelete(Node*&Min,Node*&x,int &n)
{
    fibHeapDecreaseKey(Min,x,-100000);
    Node*result=fibHeapExtractMin(Min,n);
    delete result;
}
///Pentru a implementa aceasta functie, am mai adaugat in componenta nodului o variabila bool denumita searched,
///initializata la inceput cu false. Ea devine true pentru un nod X in momentul in care se apeleaza functia de cautare pentru
///arborele avand Min pointat catre acel nod X. Valoarea redevine false in momentul in care am terminat de verificat
///atat nodul, child list cat si root list. Variabila este utila in momentul in care verificam nodurile unui root list.
///Intrucat avem lista circulara dublu inlantuita, pentru a nu cauta la infinit valoarea in lista aceasta, retinem
/// unde am inceput cautarea folosind "searched".In momentul in care o root list(si toate listele child ale nodurilor din root list)
/// a fost verificata complet, valorile acelor noduri redevin false, facand astfel posibila cautarea si a altor valori in heap.

Node* fibHeapSearchValue(Node*&Min, int value)
{
    Node*found=NULL;
    Node *x=Min;
    x->searched=true; ///marcam faptul ca, pentru nodul Min, a fost apelata functia de cautare;
    if(x->data==value)
    {
        x->searched=false;
        found=x;
    }
    ///in cazul in care nu este egal cu minimul - adica found inca nu pointeaza catre un nod
    if(found==NULL)
    {
        if(x->child!=NULL)
        {
            Node *result=fibHeapSearchValue(x->child,value);
            found=result;
        }

        if(found==NULL && x->right->searched==false)
        {
            Node *result=fibHeapSearchValue(x->right,value);
            found=result;
        }

    }
    x->searched=false;
    return found;

}
void fibHeapDecreaseKeyExample()
{

    ///Am verificat functionalitatea operatiei de delete min luand ca heap
    ///un exemplu din Introduction to Algorithms - ch.20 p. 491
    Node*Min;
    int n;
    FibonacciHeap(Min,n);
    fibHeapInsert(Min,7,n);
    fibHeapInsert(Min,18,n);
    fibHeapInsert(Min,38,n);
    fibHeapChild(Min,Min,24,n);
    fibHeapChild(Min,Min,17,n);
    fibHeapChild(Min,Min,23,n);
    fibHeapChild(Min,Min->child,26,n);
    fibHeapChild(Min,Min->child,46,n);
    fibHeapChild(Min,Min->child->right,30,n);
    fibHeapChild(Min,Min->child->child,35,n);
    Min->child->child->mark=true;
    cout<<"Initial Heap: \n";
    fibHeapDisplay(Min,n);
    Node*c=Min->child->child->right;
    fibHeapDecreaseKey(Min,c,15);
    cout<<"\nDecrease 46 to 15\n";
    fibHeapDisplay(Min,n);
    cout<<"\nDecrease 35 to 5\n";
    c=Min->child->child->child;
    fibHeapDecreaseKey(Min,c,5);
    fibHeapDisplay(Min,n);

}
void fibHeapExtractMinExample()
{
    ///Am verificat functionalitatea operatiei de extract min luand ca heap
    ///un exemplu din Introduction to Algorithms - ch.20 p. 484
    Node*Min;
    int n;
    FibonacciHeap(Min,n);
    fibHeapInsert(Min,3,n);
    fibHeapInsert(Min,17,n);
    fibHeapInsert(Min,24,n);
    fibHeapInsert(Min,23,n);
    fibHeapInsert(Min,7,n);
    fibHeapInsert(Min,21,n);
    fibHeapChild(Min,Min,18,n);
    fibHeapChild(Min,Min,52,n);
    fibHeapChild(Min,Min,38,n);
    fibHeapChild(Min,Min->child,39,n);
    fibHeapChild(Min,Min->child->left,41,n);
    fibHeapChild(Min,Min->right,30,n);
    fibHeapChild(Min,Min->right->right,26,n);
    fibHeapChild(Min,Min->right->right->child,35,n);
    fibHeapChild(Min,Min->right->right,46,n);
    Node*result=fibHeapExtractMin(Min,n);
    cout<<"Minimul initial a fost "<<result->data<<" . Acum minimul este "<<Min->data<<" si heap-ul: \n";
    delete result;
    fibHeapDisplay(Min,n);

}
void fibHeapUnionExample()
{
    Node*Min,*Min2,*Min3;
    int n,n2,n3;
    FibonacciHeap(Min,n);
    fibHeapInsert(Min,3,n);
    fibHeapInsert(Min,17,n);
    fibHeapInsert(Min,24,n);
    fibHeapInsert(Min,23,n);
    fibHeapInsert(Min,7,n);
    fibHeapInsert(Min,21,n);
    fibHeapChild(Min,Min,18,n);
    fibHeapChild(Min,Min,52,n);
    fibHeapChild(Min,Min,38,n);
    fibHeapChild(Min,Min->child,39,n);
    fibHeapChild(Min,Min->child->left,41,n);
    fibHeapChild(Min,Min->right,30,n);
    fibHeapChild(Min,Min->right->right,26,n);
    fibHeapChild(Min,Min->right->right->child,35,n);
    fibHeapChild(Min,Min->right->right,46,n);
    cout<<"Primul heap este:\n";
    fibHeapDisplay(Min,n);


    FibonacciHeap(Min2,n2);
    fibHeapInsert(Min2,992,n2);
    fibHeapInsert(Min2,-17,n2);
    fibHeapChild(Min2,Min2,19,n2);
    cout<<"Al doilea heap este:\n";
    fibHeapDisplay(Min2,n2);
    cout<<"\nRezultatul este operatiei merge este:\n";
    fibHeapUnion(Min,Min2,Min3,n,n2,n3);
    fibHeapDisplay(Min3,n3);
}

///Referinte: T.H. Cormen, C.E. Leiserson, R.L. Rivest, C. Stein, “Fibonacci Heaps”
///in Introduction to Algorithms, 2nd ed., Cambridge, MA: The MIT Press, 2001, ch. 20, pp.476-497
///https://www.geeksforgeeks.org

int main()
{

    Node *Min;
    int n,N;
    int answer,value;

    cout<<"Introdu numarul de operatii: \n";
    cin>>N;
    FibonacciHeap(Min,n);
    for(int i=0; i<N; i++)
    {

        cout<<"Inserare(1), delete(2),find min(3) sau delete min(4)?\n";
        try
        {
            cin>>answer;
            switch(answer)
            {
            case 1:
                cin>>value;
                fibHeapInsert(Min,value,n);
                fibHeapDisplay(Min,n);
                break;
            case 2:
            {
                cin>>value;
                try
                {
                    Node*node_searched=fibHeapSearchValue(Min,value);
                    if(node_searched==NULL)
                        throw(MyException("Nodul introdus nu se afla in Heap",2));
                    fibHeapDelete(Min,node_searched,n);
                    fibHeapDisplay(Min,n);

                }
                catch(MyException&e)
                {
                    throw;
                }
                break;
            }
            case 3:
            {
                cout<<"Minimul este: "<<findMin(Min)<<"\n";
                break;
            }
            case 4:
            {

                Node*result=fibHeapExtractMin(Min,n);
                cout<<"Nodul "<<result->data<<" a fost eliminat\n";
                delete result;
                fibHeapDisplay(Min,n);
                break;
            }
            default:
                throw(MyException("Input gresit",0));

            }
        }
        catch(MyException&e)
        {
            cout<<e.what()<<"\n";
        }
    }

    try
    {
        cout<<"Vrei sa vezi si operatia de build?Introdu 1 in acest caz,0 daca nu.\n";
        cin>>answer;
        switch(answer)
        {
        case 1:
        {

            cout<<"Introdu un vector de numere - numar de elemente si valori.\n";
            int length,element;
            Node *Min1;
            cin>>length;
            vector <int> v;
            for(int i=0; i<length; i++)
            {
                cin>>element;
                v.push_back(element);
            }
            Min1=fibHeapBuild(v);
            fibHeapDisplay(Min1,length);
            break;

        }
        case 0:
            cout<<"Cum vrei tu. . . \n";
            break;
        default:
            throw(MyException("Input gresit",2));
        }

        cout<<"Vrei sa vezi si operatia de merge?Introdu 1 pentru a vedea un exemplu predefinit,2 pentru a construii\
\ 2 heapuri pe care sa le unesti, 3 daca nu vrei oricare din cele 2 optiuni.\n";
        cin>>answer;
        switch(answer)
        {
        case 1:
            fibHeapUnionExample();
            break;
        case 2:
        {

            cout<<"Introdu primul vector de numere - numar de elemente si valori.\n";
            int length,element;
            Node *Min1;
            cin>>length;
            vector <int> v;
            for(int i=0; i<length; i++)
            {
                cin>>element;
                v.push_back(element);
            }
            Min1=fibHeapBuild(v);

            cout<<"Introdu al doilea vector de numere - numar de elemente si valori.\n";
            int length2,element2;
            Node *Min2;
            cin>>length2;
            vector <int> v2;
            for(int i=0; i<length2; i++)
            {
                cin>>element2;
                v2.push_back(element2);
            }
            Min2=fibHeapBuild(v2);

            Node*Min3;
            int length3;
            fibHeapUnion(Min1,Min2,Min3,length,length2,length3);
            fibHeapDisplay(Min3,length3);
            break;
        }
        case 3:
            cout<<"Cum vrei tu. . . \n";
            break;
        default:
            throw(MyException("Input gresit",2));
        }

    }
    catch(MyException&e)
    {
        cout<<e.what()<<"\n";
    }
    return 0;
}
