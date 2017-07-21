import { fromJS, Map, List } from 'immutable';
import { isEqual } from 'lodash/lang';
import { CRUD } from './actions';

function recent(fetchedAt) {
  if (fetchedAt === null) return false;

  const interval = 10 * 60 * 1000; // 10 minutes
  return ((Date.now() - interval) < fetchedAt);
}

export function selectCollection(modelName, crud, params) {
  const model = crud.getIn([modelName], Map());
  const collection = model.get('collections', List()).find(collection => (
    isEqual(collection.get('params').toJS(), params)
  ));

  const isLoading = ({ needsFetch }) => ({
    otherInfo: {},
    data: [],
    isLoading: true,
    ...(collection ? { error: collection.get('error') } : {}),
    needsFetch
  });

  if (collection === undefined) {
    return isLoading({ needsFetch: true });
  }

  const fetchedAt = collection.get('fetchedAt');
  if (fetchedAt === 0) {
    return isLoading({ needsFetch: false });
  } else if (!recent(fetchedAt)) {
    return isLoading({ needsFetch: true });
  }

  let documentThatNeedsFetch = null;
  collection.get('ids', fromJS([])).forEach((id) => {
    const document = model.getIn(['byId', id], Map());
    const documentFetchedAt = document.get('fetchedAt');
    if (documentFetchedAt !== 0 && !recent(document.get('fetchedAt'))) {
      documentThatNeedsFetch = document;
      return false;
    }
  })
  if (documentThatNeedsFetch) {
    return isLoading({ needsFetch: true });
  }

  const data = collection.get('ids', fromJS([])).map((id) => 
    model.getIn(['byId', id, 'document'])
  ).toJS();

  return {
    otherInfo: collection.get('otherInfo', Map()).toJS(),
    data,
    isLoading: false,
    needsFetch: false,
    ...(collection ? { error: collection.get('error') } : {})
  }
}

export function selectDocument(modelName, id, crud, params) {
  const model = crud.getIn([modelName, 'byId', id]);

  if (model && model.get('fetchedAt') === 0) {
    return { 
      isLoading: true, 
      needsFetch: false, 
      error: new Error('Loading...') 
    }
  }

  if (id === undefined || model == undefined || !recent(model.get('fetchedAt'))) {
    return { 
      isLoading: true, 
      needsFetch: true, 
      error: new Error('Loading...') 
    }
  }

  if (model.get('error') !== null) {
    return { 
      isLoading: false, 
      needsFetch: false, 
      error: model.get('error') 
    }
  }

  return { 
    isLoading: false, 
    needsFetch: false, 
    data: model.get('document').toJS()
  }
}

export function select(action, crud) {
  const model = action.meta.model;
  const params = action.meta.params;

  let selection = {};
  switch (action.type) {
    case CRUD.FETCH:
      selection = selectCollection(model, crud, params);
      break;
    case CRUD.FETCH_ONE:
      if (action.meta.id === undefined) {
        return selection;
      }
      selection = selectDocument(model, action.meta.id, crud, params);
      break;
    default:
      throw new Error(`Action type '${action.type}' is not a fetch action.`);
  }
  selection.fetch = action;
  return selection;
}

export function selectActionStatus(modelName, crud, action) {
  const rawStatus = (crud.getIn([modelName, 'actionStatus', action]) || fromJS({})).toJS();
  const { pending = false, id = null, isSuccess = null, payload = null } = rawStatus;

  if (pending === true) {
    return { id, pending }
  }

  if (isSuccess === true) {
    return {
      id,
      pending,
      response: payload
    }
  }

  return {
    id,
    pending,
    error: payload
  }
}