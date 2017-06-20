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
      params,
    },
    payload: {
      method: 'get',
      url,
      params
    }
  }
}
