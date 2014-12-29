function pi = strategy(G,S,pi)

A=S;
%A_new= intersect(S,G.Q_list);
S= intersect(S,G.Q_list);
A_new=[];
A_adv=[];
A_adv_new=[];

while (~isequal(A,A_new) || ~isequal(A_adv,A_adv_new))
    Cand=[];
    A=A_new;
    A_adv=A_adv_new;
    for i=setdiff(G.Q_adv_list,A_adv) 
       succ=find(G.delta_adv(i,:));
       if ~isempty(succ)
           if isequal(succ,intersect(succ,union(A,S))) 
               A_adv_new=[A_adv_new,i];
           end
       end
    end
    
    for i=setdiff(G.Q_list,A)
        for action=find(G.delta(i,:)); 
           succ = find(G.Q_adv(:,1)==i & G.Q_adv(:,2)==action); 
           if ~isempty(intersect(succ,A_adv_new))
               A_new=[A_new,i];
               if pi(i)==0 
                   pi(i)=action;
               end
           end
        end
    end    
end
