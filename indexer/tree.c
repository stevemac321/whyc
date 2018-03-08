#include "tree.h"
/* internal helpers*/
struct tnode *insert(struct tnode *p, struct entry *pe)
{
        int cond;

	if(p == NULL) {
                p = malloc(sizeof(struct tnode));
                p->data = pe;
                p->left = NULL;
                p->right = NULL;
        }else if((cond = cmp(p->data->verse, pe->verse)) > 0)
                p->left = insert(p->left, pe);
        else
                p->right = insert(p->right, pe);

        return p;

}
void print(struct tnode *p)
{
        if(p) {
                print(p->left);
                fprintf(stdout, "%-*s %*s\n", 14, p->data->verse, 8, p->data->ref);
                print(p->right);
        }

}
void inner_delete(struct tnode *p)
{
        if(p) {
                inner_delete(p->left);
                inner_delete(p->right);
                free(p->data);
                free(p);
        }
}
/* public implementations*/
void tree_init(struct tree *t)
{
	t->root = NULL;
}
void tree_insert(struct tree *t, struct entry *pe)
{
        t->root = insert(t->root, pe);
}
void tree_delete(struct tree *t)
{
        inner_delete(t->root);
}
void tree_print(struct tree * t)
{
        print(t->root);
}
int cmp(const char *vl, const char *vr)
{
	char *pl = (char*)vl;
	char *pr = (char *)vr;

	while (*pl != ':' && *pl != '\0')
		pl++;

	while (*pr != ':' && *pr != '\0')
		pr++;

	if (*pl == ':') 
		*pl = '\0';

	if (*pr == ':') 
		*pr = '\0';
        
	int il = atoi(vl);
	int ir = atoi(vr);

        *pl = ':';
        *pr = ':';

	if (il != ir)
		return il - ir;
	else {
		pl++;
		pr++;

		char *ls = pl;
		char *rs = pr;

		while (isdigit(*pl))
			++pl;

		while (isdigit(*pr))
			++pr;

                char tl = *pl;
                char tr = *pr;
		*pl = '\0';
		*pr = '\0';

		il = atoi(ls);
		ir = atoi(rs);
                *pl = tl;
                *pr = tr;
		return il - ir;
	}
}

/*-----------queue----------------*/

void queue_init(struct queue *q)
{
        q->head = q->tail = NULL;
}
void queue_enqueue(struct queue *q, const char *line)
{
        struct qnode *pnew = malloc(sizeof(struct qnode));
        pnew->line = line;
        pnew->next = NULL;
        if(q->tail == NULL ) {
                q->head = pnew;
                q->tail = pnew;
        } else {
                q->tail->next = pnew;
                q->tail = pnew;
        }
}
const char *queue_dequeue(struct queue *q, const char *out)
{
        out = q->head->line;
        struct qnode * del = q->head;
        q->head = del->next;
        free(del);

        if(q->head == NULL)
                q->tail = NULL;

        return out;
}
bool queue_is_empty(struct queue *q)
{
        return q->head == NULL;
}
void queue_free(struct queue *q)
{
        struct qnode *p = q->head;
        while(p) {
                struct qnode * del = p;
                p = p->next;
                free(del);
        }
}
