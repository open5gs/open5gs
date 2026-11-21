import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';

import AddIcon from 'react-icons/lib/md/add';

const Wrapper = styled.div`
  position: fixed;
  bottom: ${props => props.bottom || '2rem'};
  right: 2rem;
  width: 4rem;
  height: 4rem;

  border: 1px solid ${oc.pink[9]};
  color: white;
  background: ${oc.pink[8]};

  border-radius: 2rem;
  font-size: 2rem;
  cursor: pointer;

  display: flex;
  align-items: center;
  justify-content: center;

  transition: all .15s;

  &:hover {
    transform: translateY(-0.5rem); 
    background: ${oc.pink[6]}; 
  }
`;

/* A second button on the same page sits above the first: pass `bottom`, an
 * `icon` and a `title` for its tooltip. */
const FloatingButton = ({onClick, icon, bottom, title}) => {
  const Icon = icon || AddIcon;
  return (
    <Wrapper onClick={onClick} bottom={bottom} title={title}>
      <Icon/>
    </Wrapper>
  );
};

FloatingButton.propTypes = {
  onClick: PropTypes.func,
  icon: PropTypes.func,
  bottom: PropTypes.string,
  title: PropTypes.string,
}

export default FloatingButton;