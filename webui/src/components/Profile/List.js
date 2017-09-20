import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media, transitions } from 'helpers/style-utils';
import CSSTransitionGroup from 'react-transition-group/CSSTransitionGroup';

import { Layout, Blank } from 'components';
import Item from './Item';

const Wrapper = styled.div`
  display: block;
  margin: 1rem 0.5rem;
  
  ${media.mobile`
    margin: 0.5rem 0.25rem;
  `}

  .profile-enter {
    animation: ${transitions.stretchOut} .3s ease-in;
    animation-fill-mode: forwards;
  }   
    
  .profile-leave { 
    animation: ${transitions.shrinkIn} .15s ease-in;
    animation-fill-mode: forwards;
  }   
`

const propTypes = {
  profiles: PropTypes.arrayOf(PropTypes.object),
  onView: PropTypes.func,
  onEdit: PropTypes.func,
  onDelete: PropTypes.func,
}

const List = ({ profiles, deletedImsi, onView, onEdit, onDelete }) => {
  const profileList = profiles
    .sort( 
      (a,b) => {
        if(a.imsi > b.imsi) return 1;
        if (a.imsi < b.imsi) return -1;
        return 0;
      }
    )
    .map(profile =>
      <Item 
        key={profile._id}
        profile={profile}
        disabled={deletedImsi === profile.imsi}
        onView={onView}
        onEdit={onEdit}
        onDelete={onDelete} />
    );

  return (
    <Wrapper>
      <CSSTransitionGroup
        transitionName="profile"
        transitionEnterTimeout={300}
        transitionLeaveTimeout={150}>
        {profileList}
      </CSSTransitionGroup>
    </Wrapper>
  )
}

List.propTypes = propTypes;

export default List;
