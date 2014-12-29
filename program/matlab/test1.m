function P=test1()

P.Q_list=[1:4]; % list of protagonist's vertices
P.Sigma=[1,2];
P.Q_adv=[1,1;1,2;2,1;2,2;3,1;3,2;4,1;4,2]; %real names of adversary's vertices -- the first component is the name of the progagonists vertex and the second component is the name of the action
P.Q_adv_list=[1:8]; %1 is 1,1; 2 is 1,2; 3 is 2,1 etc.

P.Cond={[3,4];[2]}; %Rabin acceptance condition -- members are protagonist's vertices

st_no=size(P.Q_list,2); %number of the protagonist's states
st_no_adv=size(P.Q_adv,1); %number of the adversary's states

P.delta=sparse(st_no,size(P.Sigma,2)); %delta function for the protagonist
P.delta_adv=sparse(st_no_adv,st_no); %delta function for the adversary

P.delta(1,1)=1; %from protagonist's state 1, action 1 is enabled
P.delta(2,1)=1; %from protagonist's state 2, action 1 is enabled
P.delta(2,2)=1; %from protagonist's state 2, action 2 is enabled
P.delta(3,1)=1; %from protagonist's state 3, action 1 is enabled
P.delta(4,1)=1; %from protagonist's state 4, action 1 is enabled

P.delta_adv(1,2)=1; %adversary can go from adversary's state 1 (which is (1,1) to protagonist's state 2
P.delta_adv(4,3)=1; %adversary can go from adversary's state 4 (which is (2,2) to protagonist's state 3
P.delta_adv(5,2)=1; %adversary can go from adversary's state 5 (which is (3,1) to protagonist's state 2
P.delta_adv(3,4)=1; %...
P.delta_adv(7,4)=1; %...


    