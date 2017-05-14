module.exports = (sequelize, DataTypes) => {
  const Group = sequelize.define('Group', {
    groupname: DataTypes.STRING
  }, {
    classMethods: {
      associate: models => {
        Group.hasMany(models.User);
      }
    }
  });

  return Group;
};