%% @doc Erlang mini project.
-module(add).
-export([m/0, n/0, o/0, b/0, start/3, start/4, padd/2]).

%Work?

m() -> start(123456789123456789, 345678912345648912, 10).
n() ->start(123456789123456789, 945678912345648912, 10,
            [{actors, 10}, {sleep, {250, 500}}, {seq, true}]).
o() ->start(123456789123456789, 945678912345648912, 10 ,
            [{actors, 2}, {sleep, {500, 1000}}, {seq, false}]).
b() -> start(1011, 1100, 2, []).



%% split(L, N) when length(L) < N ->
%%     L;

%% %% Do the splitting
%% split(L, N) ->
%%     split(L, N, []).

%% %% An auxiliary recursive split function
%% split(L, N, Lists) ->
%%     {L1, L2} = lists:split(N, L),
%%     if length(L2) > N ->
%% 	    split(L2, N, [L1|Lists]);
%%        true ->
%% 	    [L2, L1|Lists]
%%     end.


%% Seperate each individual number into an element in a list
sep(Num) ->
    N1 = divide(integer_to_list(Num), []),
    N2 = lists:map(fun(X) -> {Int, _} = string:to_integer(X), Int end, N1),
    lists:flatten(divide(N2, [])).

%% sep(0, Acc, _) -> Acc;
%% sep(N, Acc, Base) ->
%%     Next = N rem Base,
%%     io:format("Kvar  N ~p~n", [N]),
%%     io:format("Next ~p~n", [Next]),
%%     sep(trunc(N div Base), [Next | Acc], Base).

%% sep(N, Base) -> sep(N, [], Base).


divide([], L) -> L;
divide([H|T], L) ->
    divide(T, [[H]|L]).


%% Insert padding in the list if not of equal length
padd(A, B) when length(A) =:= length(B) ->
    {A, B};
padd(A, B) when length(A) >= length(B) ->
    padd(A, [0 | B]);
padd(A, B) ->
    padd([0 | A], B).


%% divisible(M, N) when M / N =:= 0 ->
%%     M / N;
%% divisible(M, N) ->
%%     N + 1.


% Makes sure PCount is never larger than Listlen
checkPCount(PCount, ListLen) when PCount > ListLen -> ListLen;
checkPCount(PCount, _) -> PCount.


% Sleep for a random amount of milliseconds in an interval
randomSleep({0,0}) -> ok;
randomSleep({Min, Max}) -> timer:sleep(Min + random:uniform(Max - Min + 1) - 1).



worker([H | []], Ppid, Count, Base, SleepLimits) ->
    {Cout, Sum} = calc(H, 0, [], Base, SleepLimits),
    %%io:format("Worker slut i listan: ~p ~n", [H]),
    %%io:format("Count: ~p Sum: ~p ~n", [Count], [Sum]),
    Ppid ! {sum, Sum , Count},
    %%io:format("Count: ~p ~n", [Count]),
    exit(Cout);

worker([Head | Pairs], Ppid, Count, Base, SleepLimits) ->

    process_flag(trap_exit,true),
    spawn_link(fun() -> worker(Pairs, Ppid,Count+1, Base, SleepLimits) end),

    {Cout0, Sum0} = calc(Head, 0, [], Base, SleepLimits),
    {Cout1, Sum1} = calc(Head, 1, [], Base, SleepLimits),
    %%io:format("Count: ~p ~n", [Head]),
    receive
        {'EXIT', _PID, no_carry} ->
            Ppid ! {sum, Sum0 , Count},
            %%io:format("Count: ~p ~n", [Count]),
            exit(Cout0);
        {'EXIT', _PID, carry} ->
            Ppid ! {sum, Sum1 , Count},
            %%io:format("Count: ~p ~n", [Count]),
            exit(Cout1)
    end.



worker_seq([H | []], Ppid, Count, Base, SleepLimits) ->
    {Cout, Sum} = calc(H, 0, [], Base, SleepLimits),
    %%io:format("Worker slut i listan: ~p ~n", [H]),
    %%io:format("Count: ~p Sum: ~p ~n", [Count], [Sum]),
    Ppid ! {sum, Sum , Count},
    %%io:format("Count: ~p ~n", [Count]),
    exit(Cout);

worker_seq([Head | Pairs], Ppid, Count, Base, SleepLimits) ->

    process_flag(trap_exit,true),
    spawn_link(fun() -> worker_seq(Pairs, Ppid,Count+1, Base, SleepLimits) end),

    %%io:format("Count: ~p ~n", [Head]),
    receive
        {'EXIT', _PID, no_carry} ->
            {Cout0, Sum0} = calc(Head, 0, [], Base, SleepLimits),
            Ppid ! {sum, Sum0 , Count},
            %%io:format("Count: ~p ~n", [Count]),
            exit(Cout0);
        {'EXIT', _PID, carry} ->
            {Cout1, Sum1} = calc(Head, 1, [], Base, SleepLimits),
            Ppid ! {sum, Sum1 , Count},
            %%io:format("Count: ~p ~n", [Count]),
            exit(Cout1)
    end.



calc([], Cout, Sum, _, _) ->
    case Cout =:= 1 of
        true ->
            {carry, Sum};
        false ->
            {no_carry, Sum}
    end;

calc([{A, B} | T], Cin, Sum, Base, SleepLimits) ->
    Tot = A + B + Cin,
    case Tot >= Base of
        true ->
            randomSleep(SleepLimits),
            calc(T, 1, [Tot rem Base | Sum], Base, SleepLimits);
        false ->
            randomSleep(SleepLimits),
            calc(T, 0, [Tot rem Base | Sum], Base, SleepLimits)
    end.


%% Sort the received sums in correct order and return correct total sum
sort(Map, List, 0) ->
    lists:concat(maps:get(0, Map) ++ List);

sort(Map, List, Pos) ->
    sort(Map, maps:get(Pos, Map) ++ List, Pos-1).



finish(List, CarryString) ->
    Map = maps:from_list(List),
    Sorted = sort(Map, [], maps:size(Map)-1),

    {Final, _} = string:to_integer(CarryString ++ Sorted),
    io:format("Correct result : ~p~n", [Final]).


loop(List) ->
    receive
        {sum, Sum, Pos}->
            %%io:format("Received in loop: ~p~n", [Pos]),
            loop([{Pos,Sum} | List]);

        {'EXIT', _PID, carry} ->
            finish(List, "1");

        {'EXIT', _PID, no_carry} ->
            finish(List, "0")
    end.


%% @doc TODO: add documentation
-spec start(A,B,Base) -> ok when
      A::integer(),
      B::integer(),
      Base::integer().

start(ArgA, ArgB, Base) ->

    PairsRev = lists:reverse(utils:split(lists:zip(sep(ArgA), sep(ArgB)), 1)),
    Pairs = lists:map(fun lists:reverse/1, PairsRev),

    %% io:format("PairsRev: ~p~n", [PairsRev]),
    %% io:format("Pairs: ~p~n", [Pairs]),

    Ppid = self(),
    process_flag(trap_exit,true),
    spawn_link(fun() -> worker_seq(Pairs, Ppid, 0, Base, {0,0}) end),

    loop([]).


% Extracts an option if found, otherwise returns the provided default
getOption([], _, Default) ->
    Default;
getOption([{Current, Value}|_], Option, _) when Current =:= Option ->
    Value;
getOption([_|T], Option, Default) ->
    getOption(T, Option, Default).


%% @doc TODO: add documentation
-spec start(A,B,Base, Options) -> ok when
      A::integer(),
      B::integer(),
      Base::integer(),
      Option::atom() | tuple(),
      Options::[Option].

start(ArgA, ArgB, Base, Options) ->
    

    Numbers = lists:zip(sep(ArgA), sep(ArgB)),

    Sleep = getOption(Options, sleep, {500, 1000}),
    Actors = getOption(Options, actors, length(Numbers)),
    SequentialMode = getOption(Options, seq, false),

    ProcessCount = checkPCount(length(Numbers), Actors),

    PairsRev = lists:reverse(utils:split(Numbers , ProcessCount)),
    Pairs = lists:reverse(lists:map(fun lists:reverse/1, PairsRev)),

    %%io:format("PairsRev: ~p~n", [PairsRev]),
    %%io:format("Pairs: ~p~n", [Pairs]),

    Ppid = self(),
    process_flag(trap_exit,true),

    case SequentialMode of
        true ->
            spawn_link(fun() -> worker_seq(Pairs, Ppid, 0, Base, Sleep) end),
            loop([]);
        false ->
            spawn_link(fun() -> worker(Pairs, Ppid, 0, Base, Sleep) end),
            loop([])
    end.
