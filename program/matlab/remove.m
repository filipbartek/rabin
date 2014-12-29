function G = remove (P,A,A_adv)

G=P;
G.Q_list = setdiff(P.Q_list,A);
G.Q_adv_list = setdiff(P.Q_adv_list,A_adv);

for i=1:numel(A)
    state=A(i);
    G.delta(state,:)=0;
    G.delta_adv(:,state)=0;
end

for i=1:numel(A_adv)
    state=A_adv(i);
    G.delta_adv(state,:)=0;
    state=G.Q_adv(A_adv(i),1);
    action=G.Q_adv(state,2);
    G.delta(state,action)=0;
end
    
G = cut(G);