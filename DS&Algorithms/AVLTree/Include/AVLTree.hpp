#include <Windows.h>

#ifndef __AK_AVLTREE_HPP__
#define __AK_AVLTREE_HPP__

#define max(a, b) (a > b ? a : b)

namespace AK {

	class CriticalLock {
	private:
		PCRITICAL_SECTION lpCritical;

	public:
		CriticalLock(PCRITICAL_SECTION crit) : lpCritical(crit) {EnterCriticalSection(lpCritical); };
		~CriticalLock() { LeaveCriticalSection(lpCritical); };
	};

	namespace STORAGE {

		template<typename T>
		class AVLTree {
		public:
			template<typename T>
			class AVLNode {
			protected:
				AVLNode<T>* lpLeft;
				AVLNode<T>* lpRight;
				int nHeight;
				T oData;

			public:
				AVLNode(const T& data) : oData(data), lpRight(NULL), lpLeft(NULL), nHeight(1) {};

			public:
				//inline AVLNode<T>*& parent() { return lpParent; };
				inline AVLNode<T>*& left() { return lpLeft; };
				inline AVLNode<T>*& right() { return lpRight; };

				//inline const AVLNode<T>* parent() const { return lpParent; };
				inline const AVLNode<T>* left() const { return lpLeft; };
				inline const AVLNode<T>* right() const { return lpRight; };

				inline T& element() { return oData; };
				inline const T& element() const { return oData; };

				inline int& height() { return nHeight; };
				inline int height() const { return nHeight; };
			};

		private:
			//The root node for the tree.
			AVLNode<T>* lpRoot;
			AVLNode<T>* lpEmptyList;
			CRITICAL_SECTION oCriticalSection;

		public:
			AVLTree() : lpRoot(NULL), lpEmptyList(NULL) {
				InitializeCriticalSection(&oCriticalSection);
			}
			~AVLTree() {
				DeleteCriticalSection(&oCriticalSection);
				RecursiveDelete(lpRoot);
				RecursiveDelete(lpEmptyList);
			}

		protected:
			template<typename ARG>
			int IterateEntry(AVLNode<T>* entry, bool(*fIterator)(const T&, int height, ARG), ARG lpParam) const {
				if (!entry) return 0;
				if (IterateEntry<ARG>(entry->left(), fIterator, lpParam)) return 1;
				if (fIterator(entry->element(), entry->height(), lpParam)) return 1;
				if (IterateEntry<ARG>(entry->right(), fIterator, lpParam)) return 1;
				return 0;
			}

			void RecursiveDelete(AVLNode<T>* lpNode) {
				if (!lpNode) return;
				RecursiveDelete(lpNode->left());
				RecursiveDelete(lpNode->right());
				delete lpNode;
			}

//========[ https://www.geeksforgeeks.org/avl-tree-set-2-deletion/ ]========================================================================================================================
			int height(AVLNode<T>* N)
			{
				if (N == NULL)
					return 0;
				return N->height();
			}

			/* Helper function that allocates a
			   new node with the given key and
			   NULL left and right pointers. */
			AVLNode<T>* newNode(const T& key)
			{
				AVLNode<T>* node = lpEmptyList;
				if (!node)
					node = new AVLNode<T>(key);
				else {
					lpEmptyList = node->right();
					node->right() = NULL;
					node->left() = NULL;
					node->height() = 1;
				}
				return node;
			}

			void deleteNode(AVLNode<T>* node) {
				node->left() = NULL;
				node->right() = lpEmptyList;
				node->height() = 0;
				lpEmptyList = node;
			}

			// A utility function to right
			// rotate subtree rooted with y
			// See the diagram given above.
			AVLNode<T>* rightRotate(AVLNode<T>* y)
			{
				AVLNode<T>* x = y->left();
				AVLNode<T>* T2 = x->right();

				// Perform rotation
				x->right() = y;
				y->left() = T2;

				// Update heights
				y->height() = max(height(y->left()),
					height(y->right())) + 1;
				x->height() = max(height(x->left()),
					height(x->right())) + 1;

				// Return new root
				return x;
			}

			// A utility function to left
			// rotate subtree rooted with x
			// See the diagram given above.
			AVLNode<T>* leftRotate(AVLNode<T>* x)
			{
				AVLNode<T>* y = x->right();
				AVLNode<T>* T2 = y->left();

				// Perform rotation
				y->left() = x;
				x->right() = T2;

				// Update heights
				x->height() = max(height(x->left()),
					height(x->right())) + 1;
				y->height() = max(height(y->left()),
					height(y->right())) + 1;

				// Return new root
				return y;
			}

			// Get Balance factor of node N
			int getBalance(AVLNode<T>* N)
			{
				if (N == NULL)
					return 0;
				return height(N->left()) -
					height(N->right());
			}

			AVLNode<T>* insert(AVLNode<T>* node, const T& key)
			{
				/* 1. Perform the normal BST rotation */
				if (node == NULL)
					return(newNode(key));

				if (key < node->element())
					node->left() = insert(node->left(), key);
				else if (key > node->element())
					node->right() = insert(node->right(), key);
				else // Equal keys not allowed
					return node;

				/* 2. Update height of this ancestor node */
				node->height() = 1 + max(height(node->left()),
					height(node->right()));

				/* 3. Get the balance factor of this
					ancestor node to check whether
					this node became unbalanced */
				int balance = getBalance(node);

				// If this node becomes unbalanced,
				// then there are 4 cases

				// Left Left Case
				if (balance > 1 && key < node->left()->element())
					return rightRotate(node);

				// Right Right Case
				if (balance < -1 && key > node->right()->element())
					return leftRotate(node);

				// Left Right Case
				if (balance > 1 && key > node->left()->element())
				{
					node->left() = leftRotate(node->left());
					return rightRotate(node);
				}

				// Right Left Case
				if (balance < -1 && key < node->right()->element())
				{
					node->right() = rightRotate(node->right());
					return leftRotate(node);
				}

				/* return the (unchanged) node pointer */
				return node;
			}

			/* Given a non-empty binary search tree,
			return the node with minimum key value
			found in that tree. Note that the entire
			tree does not need to be searched. */
			AVLNode<T>* minValueNode(AVLNode<T>* node)
			{
				AVLNode<T>* current = node;

				/* loop down to find the leftmost leaf */
				while (current->left() != NULL)
					current = current->left();

				return current;
			}

			// Recursive function to delete a node
			// with given key from subtree with
			// given root. It returns root of the
			// modified subtree.
			AVLNode<T>* remove(AVLNode<T>* root, const T& key)
			{

				// STEP 1: PERFORM STANDARD BST DELETE
				if (root == NULL)
					return root;

				// If the key to be deleted is smaller
				// than the root's key, then it lies
				// in left subtree
				if (key < root->element())
					root->left() = remove(root->left(), key);

				// If the key to be deleted is greater
				// than the root's key, then it lies
				// in right subtree
				else if (key > root->element())
					root->right() = remove(root->right(), key);

				// if key is same as root's key, then
				// This is the node to be deleted
				else
				{
					// node with only one child or no child
					if ((root->left() == NULL) ||
						(root->right() == NULL))
					{
						AVLNode<T>* temp = root->left() ?
							root->left() :
							root->right();

						// No child case
						if (temp == NULL)
						{
							temp = root;
							root = NULL;
						}
						else // One child case
						{
							*root = *temp; // Copy the contents of
										   // the non-empty child
						}
						deleteNode(temp);
					}
					else
					{
						// node with two children: Get the inorder
						// successor (smallest in the right subtree)
						AVLNode<T>* temp = minValueNode(root->right());

						// Copy the inorder successor's
						// data to this node
						root->element() = temp->element();

						// Delete the inorder successor
						root->right() = remove(root->right(),
							temp->element());
					}
				}

				// If the tree had only one node
				// then return
				if (root == NULL)
					return root;

				// STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
				root->height() = 1 + max(height(root->left()),
					height(root->right()));

				// STEP 3: GET THE BALANCE FACTOR OF
				// THIS NODE (to check whether this
				// node became unbalanced)
				int balance = getBalance(root);

				// If this node becomes unbalanced,
				// then there are 4 cases

				// Left Left Case
				if (balance > 1 &&
					getBalance(root->left()) >= 0)
					return rightRotate(root);

				// Left Right Case
				if (balance > 1 &&
					getBalance(root->left()) < 0)
				{
					root->left() = leftRotate(root->left());
					return rightRotate(root);
				}

				// Right Right Case
				if (balance < -1 &&
					getBalance(root->right()) <= 0)
					return leftRotate(root);

				// Right Left Case
				if (balance < -1 &&
					getBalance(root->right()) > 0)
				{
					root->right() = rightRotate(root->right());
					return leftRotate(root);
				}

				return root;
			}

//========[^ https://www.geeksforgeeks.org/avl-tree-set-2-deletion/ ^]======================================================================================================================

		public:
			//Inserts the data into the tree, if it doesn't already exist.
			void insert(const T& refData) {
				//console().cout() << "Inserting - " << refData << std::endl;
				CriticalLock lock(&oCriticalSection);
				lpRoot = insert(lpRoot, refData);
			}
			//Removes the data from the tree, if it already exists.
			void remove(const T& refData) {
				//console().cout() << "Removing - " << refData << std::endl;
				CriticalLock lock(&oCriticalSection);
				lpRoot = remove(lpRoot, refData);
			}

			//Checks to see if the data is alread contained within the list.
			bool contains(const T& refData) {
				CriticalLock lock(&oCriticalSection);
				return IterateEntry<const T&>(lpRoot, [](const T& entry, int, const T& other) { return entry == other;} , refData);
			}

		public:
			template<typename ARG=LPVOID>
			void Iterate(bool(*fIterator)(const T&, int height, ARG), ARG lpParam) const {
				if (!fIterator) return;
				CriticalLock lock((PCRITICAL_SECTION)&oCriticalSection);
				IterateEntry<ARG>(lpRoot, fIterator, lpParam);
			}

		public:
			friend std::ostream& operator<<(std::ostream& out, const AVLTree<T>& refTree) {
				refTree.Iterate<std::ostream&>([](const T& entry, int height, std::ostream& out)->bool{
					out << entry << std::endl;
					return false;
				}, out);
				return out;
			}
		};

	}

}

#endif//__AK_AVLTREE_HPP__