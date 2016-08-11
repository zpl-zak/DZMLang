// (c) ZaKlaus 2016; Apache 2 Licensed, see LICENSE;;

static inline OBJECT *
make_application(OBJECT *Operator, OBJECT *Operands)
{
    return(make_pair(Operator, Operands));
}

static inline b32
is_application(OBJECT *Exp)
{
    return(is_pair(Exp));
}

static inline OBJECT *
op(OBJECT *Exp)
{
    return(pair_get_a(Exp));
}

static inline OBJECT *
operands(OBJECT *Exp)
{
    return(pair_get_b(Exp));
}

static inline b32
is_no_operands(OBJECT *Ops)
{
    return(is_nil(Ops));
}

static inline OBJECT *
first_operand(OBJECT *Ops)
{
    return(pair_get_a(Ops));
}

static inline OBJECT *
rest_operands(OBJECT *Ops)
{
    return(pair_get_b(Ops));
}

static inline b32
is_let(OBJECT *Exp)
{
    return(is_tagged_list(Exp, LetSymbol));
}

static inline OBJECT *
let_bindings(OBJECT *Exp)
{
    return(pair_get_a(pair_get_b(Exp)));
}

static inline OBJECT *
let_body(OBJECT *Exp)
{
    return(pair_get_b(pair_get_b(Exp)));
}

static inline OBJECT *
binding_parameter(OBJECT *Binding)
{
    return(pair_get_a(Binding));
}

static inline OBJECT *
binding_argument(OBJECT *Binding)
{
    return(pair_get_a(pair_get_b(Binding)));
}

static inline OBJECT *
bindings_parameters(OBJECT *Bindings)
{
    return(is_nil(Bindings) ?
           Nil :
           make_pair(binding_parameter(pair_get_a(Bindings)),
                     bindings_parameters(pair_get_b(Bindings))));
}

static inline OBJECT *
bindings_arguments(OBJECT *Bindings)
{
    return(is_nil(Bindings) ?
           Nil :
           make_pair(binding_argument(pair_get_a(Bindings)),
                     bindings_arguments(pair_get_b(Bindings))));
}

static inline OBJECT *
let_parameters(OBJECT *Exp)
{
    return(bindings_parameters(let_bindings(Exp)));
}

static inline OBJECT *
let_arguments(OBJECT *Exp)
{
    return(bindings_arguments(let_bindings(Exp)));
}

static inline OBJECT *
let_to_application(OBJECT *Exp)
{
    return(make_application(
        make_lambda(let_parameters(Exp),
                    let_body(Exp)),
        let_arguments(Exp)));
}