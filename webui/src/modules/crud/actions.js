export const CRUD = {
  FETCH: 'crud/FETCH',
  FETCH_SUCCESS: 'crud/FETCH_SUCCESS',
  FETCH_FAILURE: 'crud/FETCH_FAILURE',
  FETCH_ONE: 'crud/FETCH_ONE',
  FETCH_ONE_SUCCESS: 'crud/FETCH_ONE_SUCCESS',
  FETCH_ONE_FAILURE: 'crud/FETCH_ONE_FAILURE',
  CREATE: 'crud/CREATE',
  CREATE_SUCCESS: 'crud/CREATE_SUCCESS',
  CREATE_FAILURE: 'crud/CREATE_FAILURE',
  UPDATE: 'crud/UPDATE',
  UPDATE_SUCCESS: 'crud/UPDATE_SUCCESS',
  UPDATE_FAILURE: 'crud/UPDATE_FAILURE',
  DELETE: 'crud/DELETE',
  DELETE_SUCCESS: 'crud/DELETE_SUCCESS',
  DELETE_FAILURE: 'crud/DELETE_FAILURE',
  CLEAR_ACTION_STATUS: 'crud/CLEAR_ACTION_STATUS',
};

export const fetchCollection = (model, url, params = {}, options = {}) => {
  const idProperty = options.idProperty || '_id';
  return {
    type: CRUD.FETCH, 
    meta: {
      success: CRUD.FETCH_SUCCESS,
      failure: CRUD.FETCH_FAILURE,
      model,
      idProperty,
      params
    },
    payload: {
      method: 'get',
      url,
      params
    }
  }
}

export const fetchDocument = (model, id, url, params = {}, options = {}) => {
  const idProperty = options.idProperty || '_id';
  return {
    type: CRUD.FETCH_ONE, 
    meta: {
      success: CRUD.FETCH_ONE_SUCCESS,
      failure: CRUD.FETCH_ONE_FAILURE,
      model,
      idProperty,
      id
    },
    payload: {
      method: 'get',
      url,
      params
    }
  }
}

export const createDocument = (model, url, params = {}, data = {}, options = {}) => {
  const idProperty = options.idProperty || '_id';
  return {
    type: CRUD.CREATE, 
    meta: {
      success: CRUD.CREATE_SUCCESS,
      failure: CRUD.CREATE_FAILURE,
      model,
      idProperty,
    },
    payload: {
      method: 'post',
      url,
      params,
      data
    }
  }
}

export const updateDocument = (model, id, url, params = {}, data = {}, options = {}) => {
  const idProperty = options.idProperty || '_id';
  return {
    type: CRUD.UPDATE, 
    meta: {
      success: CRUD.UPDATE_SUCCESS,
      failure: CRUD.UPDATE_FAILURE,
      model,
      idProperty,
      id
    },
    payload: {
      method: 'patch',
      url,
      params,
      data
    }
  }
}

export const deleteDocument = (model, id, url, params = {}, options = {}) => {
  const idProperty = options.idProperty || '_id';
  return {
    type: CRUD.DELETE, 
    meta: {
      success: CRUD.DELETE_SUCCESS,
      failure: CRUD.DELETE_FAILURE,
      model,
      idProperty,
      id
    },
    payload: {
      method: 'delete',
      url,
      params
    }
  }
}

export const clearActionStatus = (model, action) => {
  return {
    type: CRUD.CLEAR_ACTION_STATUS,
    payload: { model, action }
  }
}