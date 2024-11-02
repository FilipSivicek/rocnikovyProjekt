/* PLUGIN file to use with plantri.c */

int FIND(int p, int* UF){
    if (UF[p] == p){
        return p;
    }
    UF[p] = FIND(UF[p], UF);
    return UF[p];
}
 
void UNION(int p1, int p2, int* UF){
    UF[FIND(p2, UF)] = FIND(p1, UF);
}

//connect vertex pos to all surrounding vertices with same colour
int connect(int pos, long long unsigned int *vertices, long long unsigned int *fixed,
    long long unsigned int *newFixed, int *UF){
    EDGE *e = firstedge[pos];
    int posColour = (*vertices&(1<<pos)) > 0;
    for (int i = 0; i < degree[pos]; i++){
        if (e->end < pos){
            if (((*vertices&(1<<e->end)) > 0) == posColour){
                if (FIND(e->end, UF) == FIND(pos, UF)){
                    return 0;
                }
                else{
                    UNION(e->end, pos, UF);
                }
            }
        }
        e = e->next;
    }
    
    for (int i = 0; i < degree[pos]; i++){
        if ((e->end < pos || (*fixed&(1<<e->end))) && (((*vertices&(1<<e->end)) > 0) == posColour)){
            if ((e->next->end < pos) || (*fixed&(1<<e->next->end))){
                if(((*vertices&(1<<e->next->end)) > 0) == posColour){
                    return 0;
                }
            }
            *newFixed = *newFixed | (1<<e->next->end);
            *vertices |= (1-posColour) << (e->next->end);
            if ((e->prev->end < pos) || (*fixed&(1<<e->prev->end))){
                if (((*vertices&(1<<e->prev->end)) > 0) == posColour){
                    return 0;
                }
            }
            *newFixed = *newFixed | (1<<e->prev->end);
            *vertices |= (1-posColour) << (e->prev->end);
        }
        e = e->next;
    }

    return 1;
}

static int generate_vertices(int pos, long long unsigned vertices, long long unsigned fixed,
    long long unsigned newFixed, int* UF){
    if (pos >= nv){
        //check if good
        int comp0 = -1;
        int comp1 = -1;

        for (int i = 0; i < nv; i++){
            if (vertices&(1<<i)){
                if (comp1 < 0){comp1 = FIND(i, UF);}
                else{
                    if (FIND(comp1, UF) != FIND(i, UF)){return 0;}
                }
            }
            else {
                if (comp0 < 0){comp0 = FIND(i, UF);}
                else {
                    if (FIND(comp0, UF) != FIND(i, UF)){return 0;}
                }
            }
        }
        return 1;
    }

    if(newFixed){
        fixed |= newFixed;
        long long unsigned int helper = newFixed;
        newFixed = 0;
        for (int i = 0; i < nv; i++){
            if (newFixed&(1<<i)){
                if(!connect(i, &vertices, &fixed, &newFixed, UF)){
                    return 0;
                }
            }
        }
        return generate_vertices(pos, vertices, fixed, newFixed, UF);
    }

    if(fixed&(1<<pos)){
        if(connect(pos, &vertices, &fixed, &newFixed, UF)){
            return generate_vertices(pos+1, vertices, fixed, newFixed, UF);
        }
        else{
            return 0;
        }
    }

    long long unsigned int copyVertices = vertices;
    long long unsigned int copyFixed = fixed;
    long long unsigned int copyNF = newFixed;
    int* copyUF = (int *) malloc(nv * sizeof(int));
    for (int i = 0; i < nv; i++){
        copyUF[i] = UF[i];
    }

    int ans = 0;

    if (connect(pos, &copyVertices, &copyFixed, &copyNF, copyUF)){
        ans = generate_vertices(pos+1, copyVertices, copyFixed, copyNF, copyUF);
    }
    free(copyUF);
    if (ans){
        return 1;
    }

    ans = 0;
    vertices |= 1<<pos;
    if (connect(pos, &vertices, &fixed, &newFixed, UF)){
        ans = generate_vertices(pos+1, vertices, fixed, newFixed, UF);
    }

    return ans;
}

#define FILTER contains_hamilton_UF_2

static int contains_hamilton_UF_2(int nbtot, int nbop, int doflip){
    int *UF = (int *) malloc(nv * sizeof(int));
    for (int i = 0; i < nv; i++){
        UF[i] = i;
    }
    int ans = !generate_vertices(1, 0, 0, 0, UF);
    free(UF);
    return ans;
}