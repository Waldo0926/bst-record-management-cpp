#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 100

typedef int Status;

typedef struct s
{
    int key;
    char name[20];
    char sex[3];
    int age;
    char zc[50];
} ElemType;

ElemType cun[100]; //初始化一维数组的长度 声明一维数组 保存数据的时候使用它
int cunchu = 0; //用来记录结构体数据的个数 即 cun[100]数组的实际长度

typedef struct Node
{
    ElemType data;
    struct Node *lchild;
    struct Node *rchild;
} BiTNode, *BiTree;

void visit(BiTree T) //将二叉排序树进行中序遍历的结果打印出来
{
    printf("%-8d%-8s%-8s%-8d%-8s\n", T->data.key, T->data.name, T->data.sex, T->data.age, T->data.zc);

}

void visit1(BiTree T, int &cunchu)  //保存前先序遍历二叉排序树 将遍历结果存入一维数组中
{

    printf("%-8d%-8s%-8s%-8d%-8s\n", T->data.key, T->data.name, T->data.sex, T->data.age, T->data.zc);
    cun[cunchu] = T->data; //将数据逐一按中序遍历结果的顺序储存到一维数组中
    cunchu++; //插入一个结构体数据后 就增加一维数组的长度 以便下次存储进去
}


void insert(BiTree &bst, ElemType teacher) //建立二叉树函数
{
    BiTree p;
    if (bst == NULL)
    {
        p = (BiTree)malloc(sizeof(BiTNode));
        p->data = teacher;
        p->lchild = NULL;
        p->rchild = NULL;
        bst = p;
    }
    else if (teacher.key < bst->data.key)
        insert(bst->lchild, teacher);
    else if (teacher.key > bst->data.key)
        insert(bst->rchild, teacher);
}

void creattree(BiTree &bst) //录入信息并建立二叉树的函数
{
    FILE *fp;
    int x, i, n;
    ElemType teacher[N], z;
    bst = (BiTree)malloc(sizeof(BiTNode));
    bst->data.key = NULL;
    bst->lchild = bst->rchild = NULL;   //以上为创建头结点
    printf("请选择使用哪种方式导入数据：（1为键盘录入，2为文件录入）");
    scanf("%d", &x);
    if (x == 1)
    {
//        fp = fopen("tea.dat", "wb");
        printf("请输入将要处理的教师人数：");
        scanf("%d", &n);
        printf("请输入%d个教师信息：\n", n);
        for (i = 0; i < n; i++)
        {
            printf("请输入第%d个教师的工号：\n", i + 1);
            scanf("%d", &teacher[i].key);
            getchar();
            printf("请输入第%d个教师的姓名：\n", i + 1);
            gets(teacher[i].name);
            printf("请输入第%d个教师的性别（男或女）：\n", i + 1);
            gets(teacher[i].sex);
            printf("请输入第%d个教师的年龄：\n", i + 1);
            scanf("%d", &teacher[i].age);
            getchar();
            printf("请输入第%d个教师的职称：\n", i + 1);
            gets(teacher[i].zc);
            cun[i] = teacher[i];
        }
        for (i = 0; i < n; i++) //二叉树
        {
            z = teacher[i];
            insert(bst->rchild, z); //将用户输入的数据逐一建成完整的二叉排序树
        }
    }

    if (x == 2) //文件导入数据
    {
        printf("从文件中导入教师信息，打印输出所有教师信息：\n");
        fp = fopen("tea.dat", "rb");
        fread(&cunchu, sizeof(int), 1, fp); //读取一维数组的长度
        printf("工号    姓名    性别    年龄    职称\n");
        //  fprintf(fp, "%d", cunchu);
        for (i = 0; i < cunchu; i++) //将一维数组的数据逐一读取
        {
            fread(&cun[i], sizeof(ElemType), 1, fp);
            printf("%-8d%-8s%-8s%-8d%-8s\n", cun[i].key, cun[i].name, cun[i].sex, cun[i].age, cun[i].zc);
        }
        for (i = 0; i < cunchu; i++)
        {
            z = cun[i];
            insert(bst->rchild, z); //将一维数组的数据 逐步建立成完整的二叉排序树
        }
        fclose(fp);
    }
    printf("教师信息已录入！\n");
}

void InOrderTraverse(BiTree T)  //使用递归的方法来中序遍历二叉排序树
{
    if (T)
    {
        InOrderTraverse(T->lchild);
        visit(T); //调用中序遍历的打印结果函数
        InOrderTraverse(T->rchild);
    }
}

void outputdata(BiTree &bst) //打印信息函数
{
    InOrderTraverse(bst); //调用中序遍历函数 对信息进行中序遍历 将结果呈现给用户
}

BiTree search(BiTree bst, int key)   //查找信息子函数
{
    if (bst == NULL)
        return NULL;
    else if (bst->data.key == key)
        return bst; //找到了之后 返回值是指针！
    else if (key < bst->data.key)
        return search(bst->lchild, key);
    else
        return search(bst->rchild, key);
}

void searchall(BiTree bst) //查询系统中已有信息的函数
{
    int key, x;
    BiTree a;
    printf("请输入需要查询的教师的工号：");
    scanf("%d", &key);
    a = search(bst, key);//调用查找信息子函数
    if (a != NULL) //a是结构体指针 通过search（）子函数的返回值（指针） 去“搜索匹配”
    {
        printf("查找成功!以下为教师的信息：\n");
        printf("%-8d%-8s%-8s%-8d%-8s\n", a->data.key, a->data.name, a->data.sex, a->data.age, a->data.zc);
    }
    if (a == NULL) //如果系统中没有这个数据的话 就将此数据插入到系统中
    {
        printf("查找失败!将此教师的信息插入系统中！\n");
        ElemType teacher;
        printf("请输入教师的工号：\n");
        scanf("%d", &teacher.key);
        getchar();
        printf("请输入教师的姓名：\n");
        gets(teacher.name);
        printf("请输入教师的性别（男或女）：\n");
        gets(teacher.sex);
        printf("请输入教师的年龄：\n");
        scanf("%d", &teacher.age);
        getchar();
        printf("请输入教师的职称：\n");
        gets(teacher.zc);
        insert(bst, teacher); //在原有二叉排序树的基础上插入子树
    }
}

void adddata(BiTree bst) //插入函数
{
    int n, i;
    ElemType teacher[N], z; //建立一个新的结构体数组 去存储插入的信息数据 再去建树
    printf("请输入将要插入的教师人数：");
    scanf("%d", &n);
    printf("请输入%d个教师信息：\n", n);
    for (i = 0; i < n; i++)
    {
        printf("请输入第%d个教师的工号：\n", i + 1);
        scanf("%d", &teacher[i].key);
        getchar();
        printf("请输入第%d个教师的姓名：\n", i + 1);
        gets(teacher[i].name);
        printf("请输入第%d个教师的性别（男或女）：\n", i + 1);
        gets(teacher[i].sex);
        printf("请输入第%d个教师的年龄：\n", i + 1);
        scanf("%d", &teacher[i].age);
        getchar();
        printf("请输入第%d个教师的职称：\n", i + 1);
        gets(teacher[i].zc);
    }
    for (i = 0; i < n; i++) //以下是将新插入的数据 在原有二叉排序树的基础上扩大规模 插入新的子树
    {
        if (search(bst, teacher[i].key) != NULL)//判断是否为系统中已有的信息数据
        {
            printf("不能插入系统中已存在的数据！");
        }
        else
        {
            //新二叉排序树的建立

            z = teacher[i];
            insert(bst->rchild, z); //调用建树函数！

        }
    }
}

void gai(BiTree bst) //修改数据的函数！
{
    int key;
    BiTree a, b;
    printf("请输入需要修改教师信息的工号：");
    scanf("%d", &key);
    getchar(); //避免“回车”的影响
    printf("\n");
    if (search(bst, key) == NULL)
    {
        printf("修改失败！系统中没有此工号的信息！请检查工号！\n");
    }
    else //对指定工号的信息一一修改 若不修改 则输入原来的信息
    {
        a = search(bst, key);
        printf("请输入姓名：\n");
        gets(a->data.name);
        printf("请输入性别（男或女）：\n");
        gets(a->data.sex);
        printf("请输入年龄：\n");
        scanf("%d", &a->data.age);
        getchar();
        printf("请输入职称：\n");
        gets(a->data.zc);

    }
}
BiTree find(BiTree bst, BiTree & parent, BiTree & q, int key) //找要删除的结点 并在此过程中构建父亲结点与孩子结点的关联关系
{
    parent = bst;
    q = bst->lchild; //从二叉排序树的根节点出发进行遍历
    while (q != NULL)
    {
        if (q->data.key == key) //刚好匹配到key值！
        {
            return q ;
        }

        parent = q;//下面的代码 q要更新为“孩子”，这个时候将“现状”赋给parent，就实现了“父亲”与“孩子”进行捆绑动态移动！

        if (key < q->data.key)//按照二叉排序树的规则去遍历！
        {
            q = q->lchild;
        }
        if (key > q->data.key)
            q = q->rchild;
    }
    return NULL;
}

//删除结点的三种情况

void del_1(BiTree parent, BiTree point)	//待删除结点为叶子结点的删除函数
{
    if (parent->rchild == point)
        parent->rchild = NULL;

    if (parent->lchild == point)
        parent->lchild = NULL;
    free(point);
}

void del_2(BiTree parent, BiTree point) //待删除结点有一个孩子的删除函数
{
    if (parent->rchild == point)
    {
        if (point->rchild != NULL)
            parent->rchild =  point->rchild;
        if (point->lchild != NULL)
            parent->rchild =  point->lchild;
    }

    if (parent->lchild ==  point)
    {
        if (point->rchild != NULL)
            parent->lchild = point->rchild;
        if (point->lchild != NULL)
            parent->lchild = point->lchild;
    }
    free(point);
}

void del_3(BiTree bst) //待删除结点有两个孩子的删除函数
{
    BiTree q;
    BiTree qfather;
    q = bst->rchild;  //q指针指向待删除结点的右孩子
    if (q->lchild == NULL)
    {
        bst->data = q->data;
        bst->rchild = q->rchild;  //这里包含了两种情况:
        //如果q有孩子，就“过继”给他的父亲——取代自己的位置; q没孩子就为空，不影响结果
        free(q);		//删除
    }
    else
    {
        while (q->lchild != NULL)  //找到“右孩子”的最左下角的结点
        {
            qfather = q;
            q = q->lchild;
        }
        bst->data = q->data;  //将最左下角结点的值复制给待删除结点
        qfather->lchild = q->rchild; //将最左下角结点的右孩子赋给自己的爸爸当左孩子（继承自己的位置），同上包含两种情况
        free(q);
    }
}

void deleteall(BiTree T) //删除结点总函数
{
    BiTree parent;
    BiTree q;
    int key;
    printf("请输入需要删除的教师信息的工号");
    scanf("%d", &key);
    parent = T;
    q = T->rchild; //从二叉排序树的根节点出发！
    while (q != NULL)
    {
        if (q->data.key == key) //匹配到key值
        {
            break;
        }

        parent = q;//下面的代码 q要更新为“孩子”，这个时候将“现状”赋给parent，就实现了“父亲”与“孩子”捆绑动态移动！

        if (key < q->data.key) //按照二叉排序树的规则去遍历！
        {
            q = q->lchild;
        }
        else
        {
            q = q->rchild;
        }
    }

    if (q == NULL)
    {
        printf("系统里没有此工号的数据，请输入有效的工号\n");
    }
    else     //已经找到了q 以下进行删除功能
    {
        if (q->lchild == NULL && q->rchild == NULL)
        {
            del_1(parent, q); //调用要删除的结点是叶子结点情况的删除函数

        }
        else if (q->lchild != NULL || q->rchild != NULL)
        {
            del_2(parent, q); //调用要删除的结点有一个孩子的情况的删除函数
        }

        else if (q->lchild != NULL && q->rchild != NULL)
        {
            del_3(q); //调用要删除的结点有两个孩子的情况的删除函数
        }
        else
        {

        }
    }
}

void FirstTraverse(BiTree bst) //先序遍历函数
{
    if (bst)
    {

        visit1(bst, cunchu);

        FirstTraverse(bst->lchild);

        FirstTraverse(bst->rchild);

    }
}

void baocun(BiTree bst) //保存函数
{
    int x;
    printf("请问您需要保存信息吗？(输入1表示确定,输入0表示放弃):");
    scanf("%d", &x);
    if (x == 1) //对信息进行保存！
    {
        FirstTraverse(bst->rchild); //将信息进行先序遍历
        FILE *fp;
        fp = fopen("tea.dat", "wb");
        fwrite(&cunchu, sizeof(int), 1, fp); //写入个数 即cun[i]一维数组的长度
        for (int i = 0; i < cunchu; i++) //将先序遍历的结果按顺序一一复制（写入）到一维数组中存储
        {
            fwrite(&cun[i], sizeof(ElemType), 1, fp);
        }
        fclose(fp);
        printf("保存成功!现在退出！\n");
    }
    else
    {
        printf("未保存!现在退出！\n");
    }
}

int main()
{
    BiTree bst;
    int num, ch;

    do
    {
        printf("\n");
        printf("**************************\n");
        printf("1:录入信息\n");
        printf("2:打印信息\n");
        printf("3:查找信息\n");
        printf("4:修改信息\n");
        printf("5:插入信息\n");
        printf("6:删除信息\n");
        printf("7:退出\n");
        printf("**************************\n");
        printf("根据菜单选择输入1-7中的任一个数字（温馨提示：必须先录入信息才能进行后续操作）：");
        scanf("%d", &ch);

        while (ch != 1 && ch != 2 && ch != 3 && ch != 4 && ch != 5 && ch != 6 && ch != 7)
        {
            printf("输入错误请重新输入1-7中的任一个字符：");
            scanf("%d", &ch);
        }
        switch (ch)
        {
            case 1://录入信息模块
            {
                creattree(bst); 
                break;
            }
            case 2://打印信息模块
            {
                cunchu = 0;
                outputdata(bst->rchild);
                break;
            }
            case 3://查找信息模块
            {
                searchall(bst->rchild);
                break;
            }
            case 4://修改信息模块
            {
                gai(bst);
                break;
            }
            case 5://插入信息模块
            {
                adddata( bst) ; 
                break;
            }
            case 6://删除信息模块
            {
                deleteall(bst);
                break;
            }
            case 7://退出模块
            {
                baocun(bst);
            }
        }
    }
    while (ch != 7);
    
    return 0;
}
