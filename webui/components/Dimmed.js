import styled from 'styled-components';
import PropTypes from 'prop-types';

const Black = styled.div`
  // Layout
  position: fixed;
  top: 0px;
  left: 0px;
  right: 0px;
  bottom: 0px;

  // Layer
  z-index: 5;

  // Color
  background: rgba(0,0,0,0.3);
`;

const Dimmed = ({visible}) => (
  <div>
    {visible && <Black/>}
  </div>
);

Dimmed.propTypes = {
  visible: PropTypes.bool
};

export default Dimmed;