
point1 = ark3d.Point(1.0,2.0,3.0)

-- x_dof = ark3d.IndependentDOF(1.1,false)

x_dof = point1:GetXDOF()

print(x_dof:GetValue())

io.flush()

