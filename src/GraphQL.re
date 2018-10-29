module Language = {
  type location = {
    .
    line: int,
    column: int,
  };
  type source = {
    .
    body: string,
    name: string,
    locationOffset: location,
  };
  type astNode;
};

module Error = {
  /**
     * Represents a GraphQL Error type
     */
  type error = {
    .
    message: string,
    locations: Js.Nullable.t(Js.Array.t(Language.location)),
    nodes: Js.Nullable.t(Js.Array.t(Language.astNode)),
    source: Js.Nullable.t(Language.source),
    positions: Js.Nullable.t(Js.Array.t(int)),
    originalError: Js.Nullable.t(Js.Exn.t),
  };
};

module Type = {
  /**
     * Represents a compiled GraphQL schema
     */
  type schema;
  type resolveInfo;
  type fieldResolver('source, 'context) =
    {
      .
      source: 'source,
      args: Js.Types.obj_val,
      context: 'context,
      info: resolveInfo,
    } =>
    Js.Types.obj_val;
};

module Execution = {
  /**
     * Represents a result from GraphQL, being either a success or a failure
     */
  type executionResult('dataObj) = {
    .
    data: 'dataObj,
    errors: Js.Nullable.t(Js.Array.t(Error.error)),
  };

  /**
     * Represents arguments being passed to the execution engine
     *
     * schema:
     *    The GraphQL type system to use when validating and executing a query.
     * source:
     *    A GraphQL language formatted string representing the requested operation.
     * rootValue:
     *    The value provided as the first argument to resolver functions on the top
     *    level type (e.g. the query object type).
     * variableCalues:
     *    A mapping of variable name to runtime value to use for all variables
     *    defined in the requestString.
     * operationName:
     *    The name of the operation to use if requestString contains multiple
     *    possible operations. Can be omitted if requestString contains only
     *    one operation.
     * fieldResolver:
     *    A resolver function to use when one is not provided by the schema.
     *    If not provided, the default field resolver is used (which looks for a
     *    value or method on the source value with the field's name).
     */
  type executionArgs(
    'rootObjType,
    'contextObjType,
    'variableValuesObjType,
    'resolverFunc,
  ) = {
    .
    "schema": Type.schema,
    "source": string,
    "rootValue": Js.Nullable.t('rootObjType),
    "contextValue": Js.Nullable.t('contextObjType),
    "variableValue": Js.Nullable.t('variableValuesObjType),
    "operationName": Js.Nullable.t(string),
    "fieldResolver": Js.Nullable.t('resolverFunc),
  };
};

module Utilities = {
  /**
     * A helper function to build a GraphQLSchema directly from a source
     * document.
     */
  [@bs.module "graphql"]
  external buildSchema : string => Type.schema = "buildSchema";
};

module Validation = {
  type validationContext;
};

/**
   * This is the primary entry point function for fulfilling GraphQL operations
   * by parsing, validating, and executing a GraphQL document along side a
   * GraphQL schema.
   *
   * More sophisticated GraphQL servers, such as those which persist queries,
   * may wish to separate the validation and execution phases to a static time
   * tooling step, and a server runtime step.
   *
   * Accepts either an object with named arguments.
   */
[@bs.module "graphql"]
external graphql :
  Execution.executionArgs(
    'rootObjType,
    'contextObjType,
    'variableValuesObjType,
    'resolverFunc,
  ) =>
  Js.Promise.t(Execution.executionResult('a)) =
  "graphql";

/**
   * Easy to use wrapper of the graphql call
   */
let run =
    (
      ~rootValue=?,
      ~contextValue=?,
      ~variableValue=?,
      ~operationName=?,
      ~fieldResolver=?,
      schema,
      source,
      (),
    ) =>
  graphql({
    "schema": schema,
    "source": source,
    "rootValue": Js.Nullable.fromOption(rootValue),
    "contextValue": Js.Nullable.fromOption(contextValue),
    "variableValue": Js.Nullable.fromOption(variableValue),
    "operationName": Js.Nullable.fromOption(operationName),
    "fieldResolver": Js.Nullable.fromOption(fieldResolver),
  });