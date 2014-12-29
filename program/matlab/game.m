function [W,W_adv,pi] = game(G,F,W,pi)

nonempty=0;
for i=1:numel(F)/2
    Good=F{1,i};
    Bad=F{2,i};
    if (~isequal(Good,0) && ~isequal(Bad,0))
        nonempty=1;
        break;
    end
end

if nonempty
    for i=1:numel(F)/2
        Good=F{1,i};
        Bad=F{2,i};
        if (~isequal(Good,0) && ~isequal(Bad,0))
           [A_E,A_adv_E] = attractor_adv(G,Bad);
           G_new=remove(G,A_E,A_adv_E);
           G_old=[];
           while ~isequal(G_new,G_old)
               G_old=G_new;
               [A_A,A_adv_A] = attractor(G_old,Good);
               H=remove(G_old,A_A,A_adv_A);
               F_H=F;
               F_H{1,i}=0; F_H{2,i}=0;
               [~,W_adv_H,pi] = game(H,F_H,W,pi);
               [W_E,W_adv_E] = attractor_adv(G_old,W_adv_H);
               G_new=remove(G_old,W_E,W_adv_E);
           end
           pi = strategy(G_new,Good,pi);
           [W_new,~]=attractor(G,G_new.Q_list);
           pi = strategy(G,G_new.Q_list,pi);
           W = union(W_new,W);
           if (~isempty(W_new))
               G_next=remove(G,W,[]);
               [W_next,~,pi] = game(G_next,F,W,pi);
               W = union(W_next,W);
           end    
        end
    end
    W_adv=setdiff(G.Q_list,W);    
else       
    W=[];
    W_adv=G.Q_list;
end