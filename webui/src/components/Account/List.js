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
}

const List = ({ accounts, deletedId, onEdit, onDelete, session }) => {
  const {
    username,
    roles
  } = session.user;

  const accountList = accounts
    .map(account =>
      <Item 
        key={account.username}
        session={session}
        account={account}
        disabled={deletedId === account.username || (roles.indexOf('admin') === -1 && account.username !== username)}
        spinner={deletedId === account.username}
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
