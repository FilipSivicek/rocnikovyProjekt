/* PLUGIN file to use with plantri.c 

   This plug-in filters out those graphs, which does
   not contain clique of size 4.
*/

static void create_AM(int AM[][nv+1]){
    for (int i = 0; i < nv + (missing_vertex >= 0); i++){
        printf("%d\n", i);
        EDGE* e = firstedge[i];
        for (int j = 0; j < degree[i]; j++){
            printf("%d ", e->end);
            AM[i][e->end] += 1;
            e = e->next;
        }
        printf("\n");
    }
}

#define FILTER has_self_edge

static int has_self_edge(int nbtot, int nbop, int doflip){
    int AM[nv+1][nv+1];
    memset(AM, 0, sizeof AM);
    create_AM(&AM);
    int n_s = 0;
    for (int i = 0; i < nv+(missing_vertex >= 0); i++){
        n_s += AM[i][i];
    }
    return n_s > 0;
}