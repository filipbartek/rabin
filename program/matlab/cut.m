function G = cut(P)

G=P;
Q_list=[];
Q_adv_list=[];

while (~isequal(Q_list,G.Q_list) || ~isequal(Q_adv_list,G.Q_adv_list))
    Q_list=G.Q_list;
    Q_adv_list=G.Q_adv_list;
    for i=1:numel(Q_list)
        state=Q_list(i);
        if isempty(find(G.delta(state,:), 1)) 
            G.Q_list=setdiff(G.Q_list,state);
            G.delta_adv(:,state)=0;
        end
    end
    
    for i=1:numel(Q_adv_list)
        state=Q_adv_list(i);
        if isempty(find(G.delta_adv(state,:), 1))
            G.Q_adv_list=setdiff(G.Q_adv_list,state);
            action=G.Q_adv(state,2);
            G.delta(G.Q_adv(state,1),action)=0;
        end
    end
end


