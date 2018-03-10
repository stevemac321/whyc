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
int cmp(const char *left, const char *right)
{
        char * dupl = strdup(left);
        char * dupr = strdup(right);
        
        int qleft[10]={0};
        int qright[10]={0};

        const char *delims = ":-,";

        size_t ldex=0;
        char *tok = strtok(dupl, delims);
        while(tok) {
                qleft[ldex++] = atoi(tok);
                tok = strtok(NULL, delims);
        }

        size_t rdex=0;
        tok = strtok(dupr, delims);
        while(tok) {
                qright[rdex++] = atoi(tok);
                tok = strtok(NULL, delims);
        }

        /* ldex and rdex now serve as count*/
        for(size_t i=0; i < ldex && i < rdex; i++)
                if(qleft[i] != qright[i])
                        return qleft[i] - qright[i];
        
        /*if one queue is longer than the other, it is greater*/
        return ldex - rdex;
}

/*-----------queue----------------*/

size_t queue_count(struct queue *q)
{
        return q->count;
}
void queue_init(struct queue *q)
{
        q->head = q->tail = NULL;
        q->count = 0;
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
        q->count++;
}
const char *queue_dequeue(struct queue *q, const char *out)
{
        out = q->head->line;
        struct qnode * del = q->head;
        q->head = del->next;
        free(del);

        if(q->head == NULL)
                q->tail = NULL;

        q->count--;

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
