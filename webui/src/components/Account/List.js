import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';

import { Layout, Blank } from 'components';
import Item from './Item';

const Wrapper = styled.div`
  display: block;
  margin: 2rem;
  
  ${media.mobile`
    margin: 0.5rem 0.25rem;
  `}
`

const propTypes = {
  accounts: PropTypes.arrayOf(PropTypes.object),
  onEdit: PropTypes.func,
  onDelete: PropTypes.func,
  search: PropTypes.string
}

const List = ({ accounts, deletedId, onEdit, onDelete, search }) => {
  const accountList = accounts
    .filter(s => s.username.indexOf(search) !== -1)
    .map(account =>
      <Item 
        key={account.username}
        account={account}
        disabled={deletedId === account.username}
        onEdit={onEdit}
        onDelete={onDelete} />
    );

  return (
    <Wrapper>
      {accountList}
    </Wrapper>
  )
}

List.propTypes = propTypes;

export default List;
