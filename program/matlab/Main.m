function P = Main(P)

P = cut(P);
F=P.Cond;
W=[];
pi=zeros(1,numel(P.Q_list));

[P.W,P.W_adv,P.pi] = game(P,F,W,pi);
