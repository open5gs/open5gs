import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media, transitions } from 'helpers/style-utils';
import CSSTransitionGroup from 'react-transition-group/CSSTransitionGroup';

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
  profiles: PropTypes.arrayOf(PropTypes.object),
  onView: PropTypes.func,
  onEdit: PropTypes.func,
  onDelete: PropTypes.func,
}

const List = ({ profiles, deletedId, onView, onEdit, onDelete }) => {
  const profileList = profiles
    .map(profile =>
      <Item 
        key={profile._id}
        profile={profile}
        disabled={deletedId === profile._id}
        onView={onView}
        onEdit={onEdit}
        onDelete={onDelete} />
    );

  return (
    <Wrapper>
      {profileList}
    </Wrapper>
  )
}

List.propTypes = propTypes;

export default List;
