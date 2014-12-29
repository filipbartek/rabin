function [A,A_adv] = attractor_adv(G,S)
%function to find player's attractor sets of nonempty set S in G
%input: graph G, set S

A=[];
A_new= intersect(S,G.Q_list);
A_adv=[];
A_adv_new=[];

while (~isequal(A,A_new) || ~isequal(A_adv,A_adv_new))
    
    A=A_new;
    A_adv=A_adv_new;
    
    for i=setdiff(G.Q_adv_list,A_adv) 
       succ=find(G.delta_adv(i,:));
       if ~isempty(intersect(succ,A))
           A_adv_new=[A_adv_new,i];
       end
    end
    
    
    for i=setdiff(G.Q_list,A)
        found=1;
        for action=find(G.delta(i,:)); 
           succ = find(G.Q_adv(:,1)==i & G.Q_adv(:,2)==action); 
           %if ~isempty(succ)
               if ~isequal(succ,intersect(succ,A_adv_new)) 
                   found=0;
               end
           %end
        end
        if found
           A_new=[A_new,i];
        end
    end    
end


A_adv=unique(A_adv);
A=unique(A);