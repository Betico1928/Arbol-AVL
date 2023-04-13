#include<iostream>  // Biblioteca para entrada y salida estándar
using namespace std; // Namespace para evitar repetir std:: en cada linea de codigo

//Estructura de nodo para nuestro árbol
struct Node{
    int val; //Valor que almacenará el nodo
    int height; //Altura del nodo en el árbol
    Node *left; //Puntero al hijo izquierdo del nodo
    Node *right; //Puntero al hijo derecho del nodo
};

//Función para obtener la altura de un nodo
int height(Node *n){
    if(n == NULL) return 0; //Si el nodo es nulo, su altura es cero
    return n->height;
}

//Función para obtener el máximo entre dos números
int max(int a, int b){
    return (a > b)? a : b;
}

//Función para crear un nuevo nodo con un valor determinado
Node* newNode(int val){
    Node* node = new Node; //Creamos un nuevo nodo con memoria dinámica
    node->val = val; //Asignamos el valor
    node->left = NULL; //Inicializamos ambos hijos como nulos
    node->right = NULL;
    node->height = 1; //Iniciamos la altura como 1, ya que un nodo nuevo siempre es una hoja
    return(node); //Devolvemos el puntero al nuevo nodo
}

//Función para realizar una rotación simple a la derecha
Node* rightRotate(Node *y){
    Node *x = y->left; //El nodo izquierdo de y será el nuevo nodo raíz
    Node *T2 = x->right; //El hijo derecho de x será el hijo izquierdo de y

    //Realizamos la rotación
    x->right = y;
    y->left = T2;

    //Actualizamos las alturas
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x; //Devolvemos el nuevo nodo raíz
}

//Función para realizar una rotación simple a la izquierda
Node* leftRotate(Node *x){
    Node *y = x->right; //El nodo derecho de x será el nuevo nodo raíz
    Node *T2 = y->left; //El hijo izquierdo de y será el hijo derecho de x

    //Realizamos la rotación
    y->left = x;
    x->right = T2;

    //Actualizamos las alturas
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y; //Devolvemos el nuevo nodo raíz
}



//Función para obtener el factor de equilibrio de un nodo
int getBalance(Node *n)
{
    if(n == NULL)
    {
        return 0; //Si el nodo es nulo, su balance es cero
    }
    return height(n->left) - height(n->right); //Calculamos la diferencia de alturas entre los hijos izquierdo y derecho
}



//Función para insertar un nuevo nodo en el árbol AVL
Node* insert(Node* node, int val)
{
    //Realizamos la inserción normal en un árbol binario de búsqueda
    if(node == NULL) return(newNode(val)); //Si el árbol está vacío, creamos un nuevo nodo con el valor dado
    if(val < node->val) node->left = insert(node->left, val); //Si el valor es menor que el valor del nodo actual, insertamos en el subárbol izquierdo
    else if(val > node->val) node->right = insert(node->right, val); //Si el valor es mayor que el valor del nodo actual, insertamos en el subárbol derecho
    else return node; //Si el valor ya está en el árbol, no hacemos nada

    //Actualizamos la altura del nodo actual
    node->height = 1 + max(height(node->left), height(node->right));

    //Obtenemos el factor de equilibrio del nodo actual
    int balance = getBalance(node);

    //Caso 1: rotación simple a la derecha
    if(balance > 1 && val < node->left->val)
        return rightRotate(node);

    //Caso 2: rotación simple a la izquierda
    if(balance < -1 && val > node->right->val)
        return leftRotate(node);

    //Caso 3: rotación doble a la derecha
    if(balance > 1 && val > node->left->val){
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    //Caso 4: rotación doble a la izquierda
    if(balance < -1 && val < node->right->val){
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    //Si el nodo está balanceado, simplemente lo devolvemos sin cambios
    return node;
}


// Funcion para encontrar el nodo con el valor mas bajo
Node* minValueNode(Node* node)
{
    Node* current = node;

    //Iteramos hasta encontrar el nodo con el valor mínimo
    while(current->left != NULL)
        current = current->left;

    return current;
}

Node* deleteNode(Node* node, int val){
    //Realizamos la eliminación normal en un árbol binario de búsqueda
    if(node == NULL) return node;
    if(val < node->val) node->left = deleteNode(node->left, val);
    else if(val > node->val) node->right = deleteNode(node->right, val);
    else{
        //Caso 1: nodo con un solo hijo o sin hijos
        if(node->left == NULL || node->right == NULL){
            Node* temp = node->left ? node->left : node->right;

            //Caso: sin hijos
            if(temp == NULL){
                temp = node;
                node = NULL;
            }
                //Caso: un hijo
            else *node = *temp;

            delete temp;
        }
            //Caso 2: nodo con dos hijos
        else{
            Node* temp = minValueNode(node->right);
            node->val = temp->val;
            node->right = deleteNode(node->right, temp->val);
        }
    }

    //Si el árbol está vacío después de la eliminación, simplemente lo devolvemos
    if(node == NULL) return node;

    //Actualizamos la altura del nodo actual
    node->height = 1 + max(height(node->left), height(node->right));

    //Obtenemos el factor de equilibrio del nodo actual
    int balance = getBalance(node);

    //Caso 1: rotación simple a la derecha
    if(balance > 1 && getBalance(node->left) >= 0)
        return rightRotate(node);

    //Caso 2: rotación simple a la izquierda
    if(balance < -1 && getBalance(node->right) <= 0)
        return leftRotate(node);

    //Caso 3: rotación doble a la derecha
    if(balance > 1 && getBalance(node->left) < 0){
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    //Caso 4: rotación doble a la izquierda
    if(balance < -1 && getBalance(node->right) > 0){
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    //Si el nodo está balanceado, simplemente lo devolvemos sin cambios
    return node;
}

// Buscar un dato
Node* search(Node* node, int val){
    //Si el nodo actual es NULL o contiene el valor buscado, lo devolvemos
    if(node == NULL || node->val == val)
        return node;

    //Si el valor buscado es mayor que el valor del nodo actual, buscamos en el subárbol derecho
    if(node->val < val)
        return search(node->right, val);

    //Si el valor buscado es menor que el valor del nodo actual, buscamos en el subárbol izquierdo
    return search(node->left, val);
}





// Funciones para imprimir los arboles
//Función para recorrer el árbol en orden
void inOrder(Node *root)
{
    if(root != NULL){
        inOrder(root->left); //Recorremos el subárbol izquierdo
        cout << root->val << " "; //Mostramos el valor del nodo actual
        inOrder(root->right); //Recorremos el subárbol derecho
    }
}

void preOrder(Node* node){
    // Si el nodo actual es NULL, regresamos
    if(node == NULL)
        return;

    // Imprimimos el valor del nodo actual
    cout << node->val << " ";
    // Llamamos a la función preOrder de forma recursiva en el subárbol izquierdo
    preOrder(node->left);
    // Llamamos a la función preOrder de forma recursiva en el subárbol derecho
    preOrder(node->right);
}

void postOrder(Node* node)
{
    if(node == NULL)
        return;

    postOrder(node->left);
    postOrder(node->right);
    cout << node->val << " ";
}





//Función principal
int main()
{
    Node *root = NULL;

    //Insertamos algunos valores
    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 30);
    root = insert(root, 40);
    root = insert(root, 50);
    root = insert(root, 25);

    // Eliminamos algunos valores
    root = deleteNode(root, 100);

    // Busca un valor
    Node* found = search(root, 100);
    if(found != NULL){
        cout << "Se encontró el valor " << found->val << endl;
    } else {
        cout << "No se encontró el valor buscado" << endl;
    }


    //Recorremos el árbol en orden
    cout << "Árbol AVL en orden: ";
    inOrder(root);
    cout << "" << endl;

    // Recorremos el árbol en pre orden
    cout << "Árbol AVL en PreOrden: ";
    preOrder(root);
    cout << "" << endl;

    // Recorremos el árbol en pre orden
    cout << "Árbol AVL en PosOrden: ";
    postOrder(root);
    cout << "" << endl;

    return 0;
}
