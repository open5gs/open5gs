import styled from 'styled-components';
import PropTypes from 'prop-types';

const Black = styled.div`
  position: fixed;
  top: 0px;
  left: 0px;
  right: 0px;
  bottom: 0px;
  z-index: ${p => p.zindex};

  background: rgba(0,0,0,0.3);
`;

const Dimmed = ({ visible, zindex }) => (
  <div>
    {visible && <Black zindex={zindex}/>}
  </div>
);

Dimmed.propTypes = {
  visible: PropTypes.bool,
  zindex: PropTypes.string
};

Dimmed.defaultProps = {
  visible: false,
  zindex: '300'
}

export default Dimmed;