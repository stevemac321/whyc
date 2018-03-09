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

        struct queue qleft;
        struct queue qright;
        const char *delims = ":-,";
        int ret = 0;

        queue_init(&qleft);
        queue_init(&qright);

        char *tok = strtok(dupl, delims);
        while(tok) {
                queue_enqueue(&qleft, tok);
                tok = strtok(NULL, delims);
        }

        tok = strtok(dupr, delims);
        while(tok) {
                queue_enqueue(&qright, tok);
                tok = strtok(NULL, delims);
        }

        const char *pl = NULL;
        const char * pr = NULL;
        int il = 0;
        int ir = 0;

        while(!queue_is_empty(&qleft) && !queue_is_empty(&qright)) {
                pl = queue_dequeue(&qleft, pl);
                pr = queue_dequeue(&qright, pr);
                il = atoi(pl);
                ir = atoi(pr);

                if(il != ir) 
                        break;
                
        }
       
        if(il != ir)
                ret = il - ir;
        else
                ret = queue_count(&qleft) - queue_count(&qright); 

        queue_free(&qleft);
        queue_free(&qright);
        return ret;
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
