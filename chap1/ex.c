#include "ex.h"
#include <stdio.h>

// 返回 stm 中的最大 print 参数个数
int maxargsStm(A_stm);
// 返回 exp 中最大 print 参数个数
int maxargsExp(A_exp);
// 返回 expList 中最大 print 参数个数
int maxargsExpList(A_expList);

int expListLen(A_expList);

// stm exp expList中都有可能包含 printStm，
// 因此，我们必须遍历所有的 stm exp ExpList
// 遇到 print stm 那么我们统计其参数个数，
// 接着，遍历其 expList 继续寻找 print stm。
// 递归终止条件：遇到 id exp，num exp，op exp 中任意一个，因为
// 这些 exp 不可能包含 print stm
int maxargs(A_stm prog) {
    return maxargsStm(prog);
}

int maxargsStm(A_stm stm) {
    int kind = stm->kind, max = 0;
    if (kind == 0 /* A_compoundStm */) {
        A_stm stm1 = stm->u.compound.stm1,
            stm2 = stm->u.compound.stm2;
        int len1 = maxargsStm(stm1), 
            len2 = maxargsStm(stm2);
        if (len1 > max) max = len1;
        if (len2 > max) max = len2;
    } else if (kind == 1 /* A_assignStm */) {
        A_exp exp = stm->u.assign.exp;
        int len = maxargsExp(exp);
        if (len > max) max = len;
    } else if (kind == 2 /* A_printStm */) {
        A_expList exps = stm->u.print.exps;
        int lenOfCurPrintStm = expListLen(exps),
            lenOfExpPrintStm = maxargsExpList(exps);
        if (lenOfCurPrintStm > max) max = lenOfCurPrintStm;
        if (lenOfExpPrintStm > max) max = lenOfExpPrintStm;
    }
    return max;
}

int maxargsExpList(A_expList exps) {
    int kind = exps->kind, max = 0;
    if (kind == 0 /* A_pairExpList */) {
        A_exp exp = exps->u.pair.head;
        A_expList tail = exps->u.pair.tail;
        int lenOfExp = maxargsExp(exp),
            lenOfTail = maxargsExpList(tail); 
        if (lenOfExp > max) max = lenOfExp;
        if (lenOfTail > max) max = lenOfTail;
    } else if (kind == 1 /* A_lastExpList */) {
        A_exp exp = exps->u.last;
        int lenOfExp = maxargsExp(exp);
        if (lenOfExp > max) max = lenOfExp;
    }
    return max;
}

int maxargsExp(A_exp exp) {
    int kind = exp->kind;
    if (3 != kind /* A_eseqExp */) {
        return 0;
    } else {
        A_stm stm = exp->u.eseq.stm;
        A_exp subExp = exp->u.eseq.exp;
        int lenOfStm = maxargsStm(stm),
            lenOfSubExp = maxargsExp(subExp),
            max = 0;
        if (lenOfStm > max) max = lenOfStm;
        if (lenOfSubExp > max) max = lenOfSubExp;
        return max;
    }
}

int expListLen(A_expList exps) {
    int kind = exps->kind;
    if (kind == 1 /* A_lastExpList */)
        return 1;
    A_expList tail = exps->u.pair.tail;
    return 1 + expListLen(tail);
}